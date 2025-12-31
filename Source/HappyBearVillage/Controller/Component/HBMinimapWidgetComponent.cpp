// Fill out your copyright notice in the Description page of Project Settings.


#include "HBMinimapWidgetComponent.h"

#include "Subsystem/VillageGenerationWorldSubsystem.h"
#include "UI/HBMinimapWidget.h"


UHBMinimapWidgetComponent::UHBMinimapWidgetComponent()
{
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

	UVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UVillageGenerationWorldSubsystem>();
	VillageGenerationSystem->OnVillageGenerated.AddUObject(this, &UHBMinimapWidgetComponent::SetMinimapTexture);
}

void UHBMinimapWidgetComponent::SetMinimapTexture()
{
	UVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UVillageGenerationWorldSubsystem>();
	UTexture2D* Texture = VillageGenerationSystem->GetMapData().ForestAsTexture2D;
	MinimapWidget->SetMinimapTexture(Texture);
}

void UHBMinimapWidgetComponent::ShowMinimapWidget()
{
	MinimapWidget->SetVisibility(ESlateVisibility::Visible);
}

void UHBMinimapWidgetComponent::HideMinimapWidget()
{
	MinimapWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UHBMinimapWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHBMinimapWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

