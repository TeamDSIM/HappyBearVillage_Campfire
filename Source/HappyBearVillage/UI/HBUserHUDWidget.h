// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameState/HBMafiaGameState.h"
#include "HBUserHUDWidget.generated.h"

class UHBUserWidget;
class AHBCharacterPlayer;
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
	void UpdateCurrentFightInfo(AHBPlayerState* InPlayerState, FDamageRankEntry InEntry, int32 InRank);

	void SetHUDVisibility(bool IsVisible, UHBUserWidget* InHUD);

protected:
	UPROPERTY()
	TObjectPtr<class UHBPhaseHUD> PhaseHUDWidget;

	UPROPERTY()
	TObjectPtr<class UHBFightHUD> FightHUDWidget;
};
