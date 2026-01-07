// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HBUserWidget.h"
#include "HBEffectToolTip.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBEffectToolTip : public UHBUserWidget
{
	GENERATED_BODY()

public:
	UHBEffectToolTip(const FObjectInitializer& ObjectInitializer);

	void SetToolTipText(const FText& InText);

protected:
	virtual void NativeConstruct() override;


public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<class UTextBlock> Description;
	
};
