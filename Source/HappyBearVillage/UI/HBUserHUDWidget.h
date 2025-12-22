// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameState/HBMafiaGameState.h"
#include "HBUserHUDWidget.generated.h"

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

protected:
	UPROPERTY()
	TObjectPtr<class UHBPhaseHUD> PhaseHUDWidget;
};
