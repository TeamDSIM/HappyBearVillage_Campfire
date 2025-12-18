// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HappyBearVillage/ProceduralGeneration/MapData/HBMapData.h"
#include "HBMapGenerator.generated.h"

class UWorld;

/**
 * 
 */
UCLASS(Blueprintable)
class HAPPYBEARVILLAGE_API UHBMapGenerator : public UObject
{
	GENERATED_BODY()

public:
	UHBMapGenerator();
	
	void GenerateField(FHBMapData InMapData, UWorld* InWorld);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	FHBMapData MapData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TSubclassOf<AActor> RoadFieldClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TSubclassOf<AActor> ForestFieldClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TArray<TObjectPtr<AActor>> FieldActors;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	float FieldElementSize = 1000.0f;
};