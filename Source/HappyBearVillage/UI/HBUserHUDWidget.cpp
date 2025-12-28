// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBUserHUDWidget.h"

#include "HBFightHUD.h"
#include "HBPhaseHUD.h"
#include "HBVoteHUD.h"
#include "Interface/HBCharacterHUDInterface.h"
#include "UI/HBNightStaminaWidget.h"

void UHBUserHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PhaseHUDWidget = Cast<UHBPhaseHUD>(GetWidgetFromName(TEXT("PhaseHUD")));
	
	FightHUDWidget = Cast<UHBFightHUD>(GetWidgetFromName(TEXT("FightHUD")));

	VoteHUDWidget = Cast<UHBVoteHUD>(GetWidgetFromName(TEXT("VoteHUD")));

	IHBCharacterHUDInterface* HUDPawn = Cast<IHBCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}

void UHBUserHUDWidget::UpdatePhase(EGamePhase NewGamePhase)
{
	
	if (NewGamePhase == EGamePhase::Fight)
	{
		FightHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else if (NewGamePhase != EGamePhase::Fight && NewGamePhase != EGamePhase::Vote)
	{
		FightHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (NewGamePhase == EGamePhase::Vote)
	{
		VoteHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if(VoteHUDWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			VoteHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
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

void UHBUserHUDWidget::UpdateVoteNum(int32 NewVoteNum)
{
	UE_LOG(LogTemp, Log, TEXT("HUDWidget Update Vote Num"));
	VoteHUDWidget->UpdateVoteNum(NewVoteNum);
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
