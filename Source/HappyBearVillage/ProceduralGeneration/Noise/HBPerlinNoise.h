// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralGeneration/Noise/HBNoiseSettings.h"
#include "UObject/NoExportTypes.h"
#include "HBPerlinNoise.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBPerlinNoise : public UObject
{
	GENERATED_BODY()
	
public:
	FORCEINLINE FHBNoiseSettings GetNoiseSettings() const { return NoiseSettings; }

	void GeneratePerlinNoise(FHBNoiseSettings InNoiseSettings);
	void DebugGeneratePerlinNoise(FHBNoiseSettings InNoiseSettings);
	void PrintNoise();

	TArray<int32> GetNoiseAs1D();
	int32 GetNoiseElement(int32 Row, int32 Col);

private:
	FVector2D GetRandomGradient(FIntVector2 Position, float Seed);

private:
	UPROPERTY(VisibleAnywhere, Category = "Noise")
	FHBNoiseSettings NoiseSettings;
	
	int32 Noise[256][256];
};
