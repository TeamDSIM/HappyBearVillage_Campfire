// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HBUserWidget.h"
#include "HBDamagedPlayerHUD.generated.h"

class AHBCharacterPlayer;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBDamagedPlayerHUD : public UHBUserWidget
{
	GENERATED_BODY()

public:
	UHBDamagedPlayerHUD(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateCurrentInfo(AHBCharacterPlayer* InPlayer);
	void UpdatePlayerColor(FLinearColor NewColor);
	void UpdatePlayerName(FString NewName);
	void UpdateTotalDamaged(float NewTotalDamaged);

protected:
	UPROPERTY()
	TObjectPtr<UImage> RankImage;

	UPROPERTY()
	TObjectPtr<UImage> PlayerColorImage;

	UPROPERTY()
	TObjectPtr<UTextBlock> PlayerNameText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> PlayerTotalDamagedText;

	UPROPERTY()
	FString PlayerNameString;	
};
