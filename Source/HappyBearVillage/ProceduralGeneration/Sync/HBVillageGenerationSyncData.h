// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralGeneration/Noise/HBNoiseSettings.h"
#include "HBVillageGenerationSyncData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FHBVillageGenerationSyncData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SyncData")
	int32 RandomStreamSeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SyncData")
	FHBNoiseSettings NoiseSettings = FHBNoiseSettings();
};
