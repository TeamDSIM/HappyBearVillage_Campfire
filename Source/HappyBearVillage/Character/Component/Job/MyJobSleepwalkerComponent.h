// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Component/Job/HBJobBaseComponent.h"
#include "MyJobSleepwalkerComponent.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UMyJobSleepwalkerComponent : public UHBJobBaseComponent
{
	GENERATED_BODY()
public:

	UMyJobSleepwalkerComponent();

protected:
	virtual void NightPhaseBegin() override;
	
};
