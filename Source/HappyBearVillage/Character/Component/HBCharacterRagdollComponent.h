// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HBCharacterRagdollComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAPPYBEARVILLAGE_API UHBCharacterRagdollComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHBCharacterRagdollComponent();

	void ApplyRagdoll();
};
