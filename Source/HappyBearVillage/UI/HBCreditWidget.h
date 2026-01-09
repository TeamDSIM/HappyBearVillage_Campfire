// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HBCreditWidget.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBCreditWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void PlayCredits();

	UFUNCTION(BlueprintCallable)
	void CloseCredits();
	
protected:

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> CreditScrollAnim;

};
