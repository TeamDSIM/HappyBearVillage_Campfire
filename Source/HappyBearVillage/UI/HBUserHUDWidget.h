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

	// Stamina UI 갱신을 블루프린트에서 구현하도록 노출
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Night")
	void UpdateStamina(int32 NewStamina);

protected:
	UPROPERTY()
	TObjectPtr<class UHBPhaseHUD> PhaseHUDWidget;
};
