// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractObject/HBInteractableActor.h"

#include "Components/SphereComponent.h"

// Sets default values
AHBInteractableActor::AHBInteractableActor()
{
	InteractionRange = 200.f;

	InteractMesh = CreateDefaultSubobject<UStaticMeshComponent>("Interact Mesh");
	RootComponent = InteractMesh;
}

bool AHBInteractableActor::CanInteract(AActor* Interactor) const
{
	return true;
}

void AHBInteractableActor::Interact(AActor* Interactor)
{
	UE_LOG(LogTemp, Log, TEXT("Interact 호출"));
	if (CanInteract(Interactor) == false)
	{
		return;
	}

	ExecuteInteraction(Interactor);
}

void AHBInteractableActor::ExecuteInteraction(AActor* Interactor)
{
}


