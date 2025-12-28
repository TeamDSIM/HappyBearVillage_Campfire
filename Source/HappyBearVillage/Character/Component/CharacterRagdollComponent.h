// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterRagdollComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAPPYBEARVILLAGE_API UCharacterRagdollComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterRagdollComponent();

	void ApplyRagdoll();
};
