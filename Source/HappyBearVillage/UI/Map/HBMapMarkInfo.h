// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HBMapMarkInfo.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FHBMapMarkInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Mark")
	FLinearColor Color;

	UPROPERTY(EditAnywhere, Category = "Mark")
	FVector2D Position;
};