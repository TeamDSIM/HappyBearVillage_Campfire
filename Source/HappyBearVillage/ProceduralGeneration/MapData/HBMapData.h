// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HBMapData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FHBMapData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
	FIntVector2 Resolution = { 64, 64 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
	int32 AreaScale = 4;

	UPROPERTY(VisibleAnywhere, Category = "MapData")
	TObjectPtr<UTexture2D> ForestTexture2D;
	
	TArray<TArray<TCHAR>> Map;
	TArray<TArray<FVector>> ForestBorderGridIndices;

	
};