// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HBUserWidget.h"
#include "HBGameEndHUD.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBGameEndHUD : public UHBUserWidget
{
	GENERATED_BODY()

public:
	UHBGameEndHUD();
	
public:
	void NativeConstruct() override;

public:
	void UpdateWinText(FString NewWinText);

public:
	UPROPERTY()
	TObjectPtr<UTextBlock> WinText;

	UPROPERTY()
	FString WinTextString;	
};
