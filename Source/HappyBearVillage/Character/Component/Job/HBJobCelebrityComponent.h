// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Component/Job/HBJobBaseComponent.h"
#include "HBJobCelebrityComponent.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBJobCelebrityComponent : public UHBJobBaseComponent
{
	GENERATED_BODY()

public:
	UHBJobCelebrityComponent();

protected:
	virtual void NightPhaseBegin() override;

	
};
