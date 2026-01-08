// Fill out your copyright notice in the Description page of Project Settings.


#include "HBMapWidgetComponent.h"

#include "HappyBearVillage.h"
#include "GameState/HBMafiaGameState.h"
#include "Subsystem/HBVillageGenerationWorldSubsystem.h"
#include "UI/Map/HBMapWidget.h"


UHBMapWidgetComponent::UHBMapWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	static ConstructorHelpers::FClassFinder<UHBMapWidget> MapWidgetClassRef(TEXT("/Game/UI/Map/WBP_Map.WBP_Map_C"));
	if (MapWidgetClassRef.Class)
	{
		MapWidgetClass = MapWidgetClassRef.Class;
	}
}

bool UHBMapWidgetComponent::IsMapValid() const
{
	UHBVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UHBVillageGenerationWorldSubsystem>();
	if (!VillageGenerationSystem->IsGenerated()) return false;
	if (!MapWidget) return false;

	return true;
}

void UHBMapWidgetComponent::CreateMapWidget(APlayerController* InPlayerController)
{
	MapWidget = CreateWidget<UHBMapWidget>(InPlayerController, MapWidgetClass);
	MapWidget->AddToViewport();
	HideMapWidget();

	UHBVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UHBVillageGenerationWorldSubsystem>();
	VillageGenerationSystem->OnVillageGenerated.AddUObject(this, &UHBMapWidgetComponent::SetMapTexture);
	VillageGenerationSystem->OnVillageGenerated.AddUObject(MapWidget, &UHBMapWidget::RefreshPlayerColorList);

	AHBMafiaGameState* HBGameState = Cast<AHBMafiaGameState>(GetWorld()->GetGameState());
	HBGameState->OnGamePhaseChanged.AddUObject(this, &UHBMapWidgetComponent::SetSyncStateByPhase);
}

void UHBMapWidgetComponent::ShowMapWidget()
{
	if (!IsMapValid()) return;
	
	//MapWidget->SetVisibility(ESlateVisibility::Visible);
	MapWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	bIsMapVisible = true;
	SetComponentTickEnabled(true);
}

void UHBMapWidgetComponent::HideMapWidget()
{
	if (!MapWidget) return;
	
	MapWidget->SetVisibility(ESlateVisibility::Hidden);
	bIsMapVisible = false;
	SetComponentTickEnabled(false);
}

void UHBMapWidgetComponent::SetOwnMarkColor(FLinearColor InOwnMarkColor)
{
	if (!MapWidget) return;
	MapWidget->SetOwnMarkColor(InOwnMarkColor);
}

void UHBMapWidgetComponent::RequestMark(FLinearColor Color, FVector2D Position)
{
	UE_LOG(LogYS, Log, TEXT("RequestMark"));
	ServerRPCMark(Color, Position);
}

void UHBMapWidgetComponent::SetSyncStateByPhase(EGamePhase Phase)
{
	UE_LOG(LogYS, Log, TEXT("SetSyncStateByPhase Begin"));
	
	if (Phase == EGamePhase::Discussion)
	{
		UE_LOG(LogYS, Log, TEXT("SetSyncStateByPhase Discussion"));
		
		MapWidget->OnClickMap.AddUObject(this, &UHBMapWidgetComponent::RequestMark);
	
		AHBMafiaGameState* HBGameState = Cast<AHBMafiaGameState>(GetWorld()->GetGameState());
		HBGameState->OnMapMarksChanged.AddUObject(MapWidget, &UHBMapWidget::RefreshMapMarks);
		
		MapWidget->ClearMapMarks();
	}
	else if (Phase == EGamePhase::Night)
	{
		UE_LOG(LogYS, Log, TEXT("SetSyncStateByPhase Night"));
		
		MapWidget->OnClickMap.RemoveAll(this);
	
		AHBMafiaGameState* HBGameState = Cast<AHBMafiaGameState>(GetWorld()->GetGameState());
		HBGameState->OnMapMarksChanged.RemoveAll(this);
		
		MapWidget->ClearMapMarks();
	}
}

void UHBMapWidgetComponent::ServerRPCMark_Implementation(FLinearColor Color, FVector2D Position)
{
	UE_LOG(LogYS, Log, TEXT("ServerRPCMark_Implementation"));
	
	AHBMafiaGameState* HBGameState = Cast<AHBMafiaGameState>(GetWorld()->GetGameState());

	TArray<FHBMapMarkInfo> NewMapMarks;

	for (FHBMapMarkInfo& MapMark : HBGameState->MapMarks)
	{
		if (MapMark.Color != Color)
		{
			NewMapMarks.Add(MapMark);
		}
	}

	NewMapMarks.Add(FHBMapMarkInfo(Color, Position));

	HBGameState->MapMarks = NewMapMarks;
	HBGameState->OnRep_MapMarks();
}

void UHBMapWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetPlayerPosition();
	SetPlayerDirection();
}

void UHBMapWidgetComponent::SetMapTexture()
{
	UHBVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UHBVillageGenerationWorldSubsystem>();
	UTexture2D* Texture = VillageGenerationSystem->GetMapData().MapTexture;
	MapWidget->SetMapTexture(Texture);
}

void UHBMapWidgetComponent::SetPlayerPosition()
{
	APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!Pawn) return;
	
	UHBVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UHBVillageGenerationWorldSubsystem>();
	if (!VillageGenerationSystem || !VillageGenerationSystem->IsGenerated()) return;
	
	FIntVector2 Resolution = VillageGenerationSystem->GetMapData().Resolution;
	int32 AreaScale = VillageGenerationSystem->GetMapData().AreaScale;
	
	FVector Location = Pawn->GetActorLocation();
	FVector NormalizedLocation = FVector(Location.X / (Resolution.X * AreaScale * 100), Location.Y / (Resolution.Y * AreaScale * 100), 0.0f);
	
	MapWidget->SetPlayerPosition(NormalizedLocation);
}

void UHBMapWidgetComponent::SetPlayerDirection()
{
	APawn* Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!Pawn) return;
	
	FVector Forward = Pawn->GetActorForwardVector();
	FVector2D Dir(Forward.X, Forward.Y);
	
	float AngleRad = FMath::Atan2(-Dir.X, -Dir.Y);
	if (AngleRad < 0.0f) AngleRad += 2.0f * PI;

	MapWidget->SetPlayerDirAngle(AngleRad);
}