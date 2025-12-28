// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/CharacterRagdollComponent.h"

#include "GameFramework/Character.h"

UCharacterRagdollComponent::UCharacterRagdollComponent() { }

void UCharacterRagdollComponent::ApplyRagdoll()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		return;
	}

	Character->GetMesh()->SetCollisionProfileName("Ragdoll");
	Character->GetMesh()->SetSimulatePhysics(true);
}
