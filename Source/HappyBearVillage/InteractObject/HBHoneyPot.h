// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractObject/HBInteractableActor.h"
#include "HBHoneyPot.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API AHBHoneyPot : public AHBInteractableActor
{
	GENERATED_BODY()

public:
	AHBHoneyPot();

protected:
	virtual void ExecuteInteraction(AActor* Interactor) override;

public:
	virtual bool CanInteract(AActor* Interactor) const override;
};
