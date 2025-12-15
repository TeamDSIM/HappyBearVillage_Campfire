// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractObject/HBInteractableActor.h"
#include "HBWeaponPickup.generated.h"

class AHBWeaponBase;
/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API AHBWeaponPickup : public AHBInteractableActor
{
	GENERATED_BODY()

public:
	AHBWeaponPickup();

protected:
	virtual void ExecuteInteraction(AActor* Interactor) override;

public:
	virtual bool CanInteract(AActor* Interactor) const override;

protected:
};
