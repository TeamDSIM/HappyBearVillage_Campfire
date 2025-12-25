// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBUserHUDWidget.h"

#include "HBFightHUD.h"
#include "HBPhaseHUD.h"
#include "Interface/HBCharacterHUDInterface.h"

void UHBUserHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PhaseHUDWidget = Cast<UHBPhaseHUD>(GetWidgetFromName(TEXT("PhaseHUD")));
	
	FightHUDWidget = Cast<UHBFightHUD>(GetWidgetFromName(TEXT("FightHUD")));
	if (FightHUDWidget)
	{
		//FightHUDWidget->RemoveFromParent();
	}

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

void UHBUserHUDWidget::UpdateDate(int32 NewDate)
{
	PhaseHUDWidget->UpdateCurrentDate(NewDate);
}

void UHBUserHUDWidget::UpdateCurrentFightInfo(AHBPlayerState* InPlayerState, FDamageRankEntry InEntry, int32 InRank)
{
	UE_LOG(LogTemp, Log, TEXT("[UHBUserHUDWidget] UpdateCurrentFightInfo InRank : %d"), InRank);
	FightHUDWidget->UpdateCurrentInfo(InPlayerState, InEntry, InRank);
}

void UHBUserHUDWidget::SetHUDVisibility(bool IsVisible, UHBUserWidget* InHUD)
{
	if (IsVisible)
	{
		InHUD->AddToViewport();
	}
	else
	{
		InHUD->RemoveFromParent();
	}
}
