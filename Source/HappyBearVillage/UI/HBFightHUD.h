// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameState/HBMafiaGameState.h"
#include "PlayerState/HBPlayerState.h"
#include "UI/HBUserWidget.h"
#include "HBFightHUD.generated.h"

class AHBCharacterPlayer;
class UHBDamagedPlayerHUD;
/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBFightHUD : public UHBUserWidget
{
	GENERATED_BODY()

public:
	UHBFightHUD(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateCurrentInfo(AHBPlayerState* InPlayer, FDamageRankEntry InEntry, int32 InRank);

protected:
	UPROPERTY()
	TObjectPtr<UHBDamagedPlayerHUD> FirstDamagedPlayerWidget;
	
	UPROPERTY()
	TObjectPtr<UHBDamagedPlayerHUD> SecondDamagedPlayerWidget;
	
	UPROPERTY()
	TObjectPtr<UHBDamagedPlayerHUD> ThirdDamagedPlayerWidget;
};
