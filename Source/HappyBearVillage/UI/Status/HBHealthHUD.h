// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HBUserWidget.h"
#include "HBHealthHUD.generated.h"

class UHBHealthSlot;
/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBHealthHUD : public UHBUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHealth(int32 NewHealth);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> HeartSlots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UHBHealthSlot> HealthSlotClass;

	
};
