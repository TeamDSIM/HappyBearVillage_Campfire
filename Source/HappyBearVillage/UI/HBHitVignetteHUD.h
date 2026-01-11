// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HBUserWidget.h"
#include "HBHitVignetteHUD.generated.h"

/**
 * 
 */

class UImage;
class UWidgetAnimation;

UCLASS()
class HAPPYBEARVILLAGE_API UHBHitVignetteHUD : public UHBUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void PlayHit(float Intensity);

protected:
	// UMG에서 Image 이름을 VignetteImage로 맞춰주세요
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> VignetteImage;

	// UMG 애니메이션 이름을 HitPulse로 맞춰주세요
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> HitPulse;
};
