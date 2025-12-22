// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "UI/HBUserWidget.h"
#include "HBPhaseHUD.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBPhaseHUD : public UHBUserWidget
{
	GENERATED_BODY()

public:
	UHBPhaseHUD(const FObjectInitializer& ObjectInitializer);

public:

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> PhaseText;

	UPROPERTY()
	TObjectPtr<UTextBlock> TimeText;

	UPROPERTY()
	FString PhaseString;
};
