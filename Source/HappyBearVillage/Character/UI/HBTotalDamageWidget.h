// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HBUserWidget.h"
#include "HBTotalDamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBTotalDamageWidget : public UHBUserWidget
{
	GENERATED_BODY()

public:
	UHBTotalDamageWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateTotalDamage(float NewTotalDamage);

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> TotalDamageText;

	UPROPERTY()
	float CurrentTotalDamage;

	
};
