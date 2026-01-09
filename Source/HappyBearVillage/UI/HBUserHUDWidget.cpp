// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HBUserHUDWidget.h"

#include "HappyBearVillage.h"
#include "HBFadeHUD.h"
#include "HBFightHUD.h"
#include "HBGameEndHUD.h"
#include "HBPhaseHUD.h"
#include "HBPoliceNoticeHUD.h"
#include "HBVoteHUD.h"
#include "Interface/HBCharacterHUDInterface.h"
#include "Status/HBHealthHUD.h"
#include "UI/HBNightStaminaWidget.h"

void UHBUserHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PhaseHUDWidget = Cast<UHBPhaseHUD>(GetWidgetFromName(TEXT("PhaseHUD")));
	
	FightHUDWidget = Cast<UHBFightHUD>(GetWidgetFromName(TEXT("FightHUD")));

	VoteHUDWidget = Cast<UHBVoteHUD>(GetWidgetFromName(TEXT("VoteHUD")));

	GameEndHUDWidget = Cast<UHBGameEndHUD>(GetWidgetFromName(TEXT("GameEndHUD")));

	FadeHUDWidget = Cast<UHBFadeHUD>(GetWidgetFromName(TEXT("FadeHUD")));

	PoliceNoticeHUDWidget = Cast<UHBPoliceNoticeHUD>(GetWidgetFromName(TEXT("PoliceNoticeHUD")));

	HealthHUDWidget = Cast<UHBHealthHUD>(GetWidgetFromName(TEXT("HealthHUD")));

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
		VoteHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
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

void UHBUserHUDWidget::UpdateGameEnd(int32 NewMafiaWin)
{
	GameEndHUDWidget->SetVisibility(ESlateVisibility::Visible);
	
	if (NewMafiaWin == 1)
	{
		GameEndHUDWidget->UpdateWinText(TEXT("MAFIA WIN"));
	}
	else if (NewMafiaWin == 2)
	{
		GameEndHUDWidget->UpdateWinText(TEXT("CITIZEN WIN"));
	}
}

void UHBUserHUDWidget::UpdateStamina(int32 NewStamina)
{
	if (NightStaminaWidget)
	{
		NightStaminaWidget->SetStamina(NewStamina);
	}
}

void UHBUserHUDWidget::UpdatePoliceNotice(bool bIsPoliceNotice)
{
	if (bIsPoliceNotice)
	{
		PoliceNoticeHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PoliceNoticeHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHBUserHUDWidget::UpdateHealth(int32 NewHealth)
{
	if (HealthHUDWidget)
	{
		UE_LOG(LogHY, Log, TEXT("HUDWidget Update Health %d"), NewHealth);
		HealthHUDWidget->UpdateHealth(NewHealth);
	}
}

void UHBUserHUDWidget::PlayFadeAnimation(bool bIsFadeIn)
{
	if (FadeHUDWidget)
	{
		if (bIsFadeIn)
		{
			FadeHUDWidget->SetVisibility(ESlateVisibility::Visible);
			FadeHUDWidget->PlayFadeIn();
		}
		else
		{
			FadeHUDWidget->PlayFadeOut();
		}
	}
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
