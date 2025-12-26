// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HBGameModeVillageGenerationComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HAPPYBEARVILLAGE_API UHBGameModeVillageGenerationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHBGameModeVillageGenerationComponent();

	void SyncVillage(int32 PlayerCount);
};
