// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractObject/HBHoneyPot.h"

AHBHoneyPot::AHBHoneyPot()
{
}

void AHBHoneyPot::ExecuteInteraction(AActor* Interactor)
{
	Super::ExecuteInteraction(Interactor);
}

bool AHBHoneyPot::CanInteract(AActor* Interactor) const
{
	return Super::CanInteract(Interactor);
}
