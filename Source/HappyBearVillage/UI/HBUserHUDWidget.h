// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameState/HBMafiaGameState.h"
#include "HBUserHUDWidget.generated.h"

class UHBUserWidget;
class AHBCharacterPlayer;
// Forward declaration for the C++ night stamina widget
class UHBNightStaminaWidget;

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBUserHUDWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

public:
	void UpdatePhase(EGamePhase NewGamePhase);
	void UpdateRemainingTime(float NewRemainingTime);
	void UpdateDate(int32 NewDate);
	void UpdateCurrentFightInfo(AHBPlayerState* InPlayerState, FDamageRankEntry InEntry, int32 InRank);
	void UpdateVoteNum(int32 NewVoteNum);
	void UpdateGameEnd(int32 NewMafiaWin);
	void UpdateStamina(int32 NewStamina);
	void UpdatePoliceNotice(bool bIsPoliceNotice);
	void UpdateHealth(int32 NewHealth);

	void PlayFadeAnimation(bool bIsFadeIn);

	void SetHUDVisibility(bool IsVisible, UHBUserWidget* InHUD);

protected:
	UPROPERTY()
	TObjectPtr<class UHBPhaseHUD> PhaseHUDWidget;

	UPROPERTY()
	TObjectPtr<class UHBFightHUD> FightHUDWidget;
	
	UPROPERTY()
	TObjectPtr<class UHBVoteHUD> VoteHUDWidget;

	UPROPERTY()
	TObjectPtr<class UHBGameEndHUD> GameEndHUDWidget;

	UPROPERTY()
	TObjectPtr<class UHBFadeHUD> FadeHUDWidget;

	UPROPERTY()
	TObjectPtr<class UHBPoliceNoticeHUD> PoliceNoticeHUDWidget;

	UPROPERTY()
	TObjectPtr<class UHBHealthHUD> HealthHUDWidget;
	
	UPROPERTY()
	TObjectPtr<class UHBHealthHUD> StaminaHUDWidget;
};
