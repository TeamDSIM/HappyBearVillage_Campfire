// Fill out your copyright notice in the Description page of Project Settings.


#include "HBMapWidgetComponent.h"

#include "Controller/HBPlayerController.h"
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
}

void UHBMapWidgetComponent::ShowMapWidget()
{
	if (!IsMapValid()) return;
	
	MapWidget->SetVisibility(ESlateVisibility::Visible);
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

void UHBMapWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetPlayerPosition();
	SetPlayerDirection();
}

void UHBMapWidgetComponent::SetMapTexture()
{
	UHBVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UHBVillageGenerationWorldSubsystem>();
	UTexture2D* Texture = VillageGenerationSystem->GetMapData().ForestAsTexture2D;
	MapWidget->SetMapTexture(Texture);
}

void UHBMapWidgetComponent::SetPlayerPosition()
{
	UHBVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UHBVillageGenerationWorldSubsystem>();
	FIntVector2 Resolution = VillageGenerationSystem->GetMapData().Resolution;
	int32 AreaScale = VillageGenerationSystem->GetMapData().AreaScale;

	FVector Location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector NormalizedLocation = FVector(Location.X / (Resolution.X * AreaScale * 100), Location.Y / (Resolution.Y * AreaScale * 100), 0.0f);
	
	MapWidget->SetPlayerPosition(NormalizedLocation);
}

void UHBMapWidgetComponent::SetPlayerDirection()
{
	FVector Forward = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorForwardVector();
	FVector2D Dir(Forward.X, Forward.Y);
	
	float AngleRad = FMath::Atan2(-Dir.X, -Dir.Y);
	if (AngleRad < 0.0f) AngleRad += 2.0f * PI;

	MapWidget->SetPlayerDirAngle(AngleRad);
}