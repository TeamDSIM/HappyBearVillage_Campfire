// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HBUserWidget.h"
#include "HBHealthSlot.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBHealthSlot : public UHBUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Heart;

	
};
