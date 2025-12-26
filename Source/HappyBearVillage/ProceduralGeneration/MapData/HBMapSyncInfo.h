// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralGeneration/Noise/HBNoiseSettings.h"
#include "HBMapSyncInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FHBMapSyncInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapSync")
	int32 RandomStreamSeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapSync")
	FHBNoiseSettings NoiseSettings = FHBNoiseSettings();
};
