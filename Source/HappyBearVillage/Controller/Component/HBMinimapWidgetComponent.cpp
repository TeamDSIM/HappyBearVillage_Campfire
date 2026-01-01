// Fill out your copyright notice in the Description page of Project Settings.


#include "HBMinimapWidgetComponent.h"

#include "Subsystem/HBVillageGenerationWorldSubsystem.h"
#include "UI/HBMinimapWidget.h"


UHBMinimapWidgetComponent::UHBMinimapWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	static ConstructorHelpers::FClassFinder<UHBMinimapWidget> MinimapWidgetClassRef(TEXT("/Game/UI/Minimap/WBP_Minimap.WBP_Minimap_C"));
	if (MinimapWidgetClassRef.Class)
	{
		MinimapWidgetClass = MinimapWidgetClassRef.Class;
	}
}

void UHBMinimapWidgetComponent::CreateMinimapWidget(APlayerController* InPlayerController)
{
	MinimapWidget = CreateWidget<UHBMinimapWidget>(InPlayerController, MinimapWidgetClass);
	MinimapWidget->AddToViewport();
	HideMinimapWidget();

	UHBVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UHBVillageGenerationWorldSubsystem>();
	VillageGenerationSystem->OnVillageGenerated.AddUObject(this, &UHBMinimapWidgetComponent::ShowMinimapWidget);
	VillageGenerationSystem->OnVillageGenerated.AddUObject(this, &UHBMinimapWidgetComponent::SetMinimapTexture);
}

void UHBMinimapWidgetComponent::ShowMinimapWidget()
{
	MinimapWidget->SetVisibility(ESlateVisibility::Visible);
	SetComponentTickEnabled(true);
}

void UHBMinimapWidgetComponent::HideMinimapWidget()
{
	MinimapWidget->SetVisibility(ESlateVisibility::Hidden);
	SetComponentTickEnabled(false);
}

void UHBMinimapWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetPlayerPosition();
	SetPlayerDirection();
}

void UHBMinimapWidgetComponent::SetMinimapTexture()
{
	UHBVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UHBVillageGenerationWorldSubsystem>();
	UTexture2D* Texture = VillageGenerationSystem->GetMapData().ForestAsTexture2D;
	MinimapWidget->SetMinimapTexture(Texture);
}

void UHBMinimapWidgetComponent::SetPlayerPosition()
{
	UHBVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UHBVillageGenerationWorldSubsystem>();
	FIntVector2 Resolution = VillageGenerationSystem->GetMapData().Resolution;
	int32 AreaScale = VillageGenerationSystem->GetMapData().AreaScale;

	FVector Location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector NormalizedLocation = FVector(Location.X / (Resolution.X * AreaScale * 100), Location.Y / (Resolution.Y * AreaScale * 100), 0.0f);
	
	MinimapWidget->SetPlayerPosition(NormalizedLocation);
}

void UHBMinimapWidgetComponent::SetPlayerDirection()
{
	FVector Forward = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorForwardVector();
	FVector2D Dir(Forward.X, Forward.Y);
	
	float AngleRad = FMath::Atan2(-Dir.X, -Dir.Y);
	if (AngleRad < 0.0f) AngleRad += 2.0f * PI;

	MinimapWidget->SetPlayerDirAngle(AngleRad);
}

