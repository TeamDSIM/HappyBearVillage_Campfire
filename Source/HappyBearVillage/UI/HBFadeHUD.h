// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HBUserWidget.h"
#include "HBFadeHUD.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBFadeHUD : public UHBUserWidget
{
	GENERATED_BODY()

public:
	UHBFadeHUD();

public:
	void NativeConstruct() override;

public:
	UFUNCTION()
	void PlayFadeIn();

	UFUNCTION()
	void PlayFadeOut();

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeIn;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;	
};
