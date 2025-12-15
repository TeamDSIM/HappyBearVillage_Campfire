// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HBInteractableInterface.h"
#include "HBInteractableActor.generated.h"

class USphereComponent;

UCLASS()
class HAPPYBEARVILLAGE_API AHBInteractableActor : public AActor, public IHBInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHBInteractableActor();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	float InteractionRange = 200.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> InteractMesh;

public:
	virtual bool CanInteract(AActor* Interactor) const override;
	virtual void Interact(AActor* Interactor) override;

protected:
	virtual void ExecuteInteraction(AActor* Interactor);
};
