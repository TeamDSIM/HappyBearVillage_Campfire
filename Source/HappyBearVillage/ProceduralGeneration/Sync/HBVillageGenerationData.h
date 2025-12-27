// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralGeneration/Noise/HBNoiseSettings.h"
#include "HBVillageGenerationData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FHBVillageGenerationData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VillageGenerationData")
	int32 RandomStreamSeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VillageGenerationData")
	FHBNoiseSettings NoiseSettings = FHBNoiseSettings();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VillageGenerationData")
	TArray<FLinearColor> HouseColorList;
};
