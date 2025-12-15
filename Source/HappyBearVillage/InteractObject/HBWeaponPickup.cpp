// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractObject/HBWeaponPickup.h"

#include "Character/HBCharacterBase.h"

AHBWeaponPickup::AHBWeaponPickup()
{
}

void AHBWeaponPickup::ExecuteInteraction(AActor* Interactor)
{
	UE_LOG(LogTemp, Log, TEXT("WeaponPickup ExecuteInteraction 호출"));
	
	AHBCharacterBase* Character = Cast<AHBCharacterBase>(Interactor);
	if (Character)
	{
		Character->EquipWeapon();
	}
}

bool AHBWeaponPickup::CanInteract(AActor* Interactor) const
{
	if (Super::CanInteract(Interactor) == false)
	{
		return false;
	}

	AHBCharacterBase* Character = Cast<AHBCharacterBase>(Interactor);
	if (Character)
	{
		if (Character->GetCurrentWeapon()->GetStaticMesh() == nullptr)
		{
			return true;
		}
	}
	
	return false;
}
