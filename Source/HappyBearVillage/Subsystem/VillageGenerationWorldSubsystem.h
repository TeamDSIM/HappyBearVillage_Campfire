// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralGeneration/Sync/HBVillageGenerationData.h"
#include "Subsystems/WorldSubsystem.h"
#include "VillageGenerationWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UVillageGenerationWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsGenerated() const { return bIsGenerated; }
	void GenerateVillage(FHBVillageGenerationData InVillageGenerationData);

private:
	UPROPERTY(VisibleAnywhere, Category = "VillageGeneation")
	uint8 bIsGenerated : 1;
	
	UPROPERTY(VisibleAnywhere, Category = "VillageGeneation")
	TObjectPtr<class UHBPerlinNoise> PerlinNoise;
	UPROPERTY(VisibleAnywhere, Category = "VillageGeneation")
	TObjectPtr<class UHBMapDataGenerator> MapDataGenerator;
	UPROPERTY(VisibleAnywhere, Category = "VillageGeneation")
	TObjectPtr<class UHBMapGenerator> MapGenerator;
};
