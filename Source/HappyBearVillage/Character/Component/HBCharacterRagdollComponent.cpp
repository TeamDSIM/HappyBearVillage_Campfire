// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/HBCharacterRagdollComponent.h"

#include "GameFramework/Character.h"

UHBCharacterRagdollComponent::UHBCharacterRagdollComponent() { }

void UHBCharacterRagdollComponent::ApplyRagdoll()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}

	Character->GetMesh()->SetCollisionProfileName("Ragdoll");
	Character->GetMesh()->SetSimulatePhysics(true);
}
