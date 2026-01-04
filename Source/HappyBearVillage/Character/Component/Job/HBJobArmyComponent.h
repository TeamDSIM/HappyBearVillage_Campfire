// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Component/Job/HBJobBaseComponent.h"
#include "HBJobArmyComponent.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBJobArmyComponent : public UHBJobBaseComponent
{
	GENERATED_BODY()

public:
	UHBJobArmyComponent();

protected:
	// 밤 시작 시점
	virtual void NightPhaseBegin() override;

private:
	
};
