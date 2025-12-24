// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Component/HBInGameHUDComponent.h"

#include "UI/HBLobbyWidget.h"
#include "UI/HBUserHUDWidget.h"

// Sets default values for this component's properties
UHBInGameHUDComponent::UHBInGameHUDComponent()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> UserHUDWidgetClassRef(
		TEXT("/Game/Personal/PARK_H_Y/UI/WBP_PlayerHUD.WBP_PlayerHUD_C"));
	if (UserHUDWidgetClassRef.Class)
	{
		UserHUDWidgetClass = UserHUDWidgetClassRef.Class;
	}
}

void UHBInGameHUDComponent::ActivateHUD(APlayerController* InPlayerController)
{
	if (!InGameHUD)
	{
		InGameHUD = CreateWidget<UHBUserHUDWidget>(InPlayerController, UserHUDWidgetClass);
		if (!InGameHUD)
		{
			UE_LOG(LogTemp, Warning, TEXT("InGameHUD can't Create"));
		}
	}
	InGameHUD->AddToViewport();
}

void UHBInGameHUDComponent::DeactivateHUD()
{
	if (InGameHUD)
	{
		InGameHUD->RemoveFromParent();
	}
}