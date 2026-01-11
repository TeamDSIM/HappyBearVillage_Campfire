// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Component/Job/HBJobBaseComponent.h"
#include "HBJobDetectorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HAPPYBEARVILLAGE_API UHBJobDetectorComponent : public UHBJobBaseComponent
{
	GENERATED_BODY()

public:
	UHBJobDetectorComponent();
	
	virtual void NightPhaseBegin() override;
	virtual void Action() override;

protected:
	void ShowTargetJob();
	

protected:
	UPROPERTY()
	uint8 bUseAbilityTonight : 1;
};
