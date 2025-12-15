// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HBNoiseSettings.generated.h"

USTRUCT(BlueprintType)
struct FHBNoiseSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	FIntVector2 Resolution = { 64, 64 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	FIntVector2 GridSize = { 4, 4 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float Seed = 0.0f;
};