// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBUserHUDWidget.h"

#include "HBPhaseHUD.h"
#include "Interface/HBCharacterHUDInterface.h"
#include "UI/HBNightStaminaWidget.h"

void UHBUserHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PhaseHUDWidget = Cast<UHBPhaseHUD>(GetWidgetFromName(TEXT("PhaseHUD")));

	IHBCharacterHUDInterface* HUDPawn = Cast<IHBCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}

void UHBUserHUDWidget::UpdatePhase(EGamePhase NewGamePhase)
{
	PhaseHUDWidget->UpdateCurrentPhase(NewGamePhase);
}

void UHBUserHUDWidget::UpdateRemainingTime(float NewRemainingTime)
{
	PhaseHUDWidget->UpdateCurrentTime(NewRemainingTime);
}