// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Component/Job/HBJobBaseComponent.h"
#include "HBJobSleepwalkerComponent.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBJobSleepwalkerComponent : public UHBJobBaseComponent
{
	GENERATED_BODY()
public:

	UHBJobSleepwalkerComponent();

protected:
	virtual void NightPhaseBegin() override;
	
};
