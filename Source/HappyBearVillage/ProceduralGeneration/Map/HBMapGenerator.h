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

	void GenerateVillage(FHBMapData InMapData, UWorld* InWorld);
	void GenerateField(FHBMapData InMapData, UWorld* InWorld);
	void GenerateHouse(FHBMapData InMapData, UWorld* InWorld);
	void GenerateForestSpline(FHBMapData InMapData, UWorld* InWorld);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	FHBMapData MapData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TSubclassOf<AActor> RoadFieldClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TSubclassOf<AActor> ForestFieldClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TArray<TSubclassOf<AActor>> HouseClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TArray<TObjectPtr<AActor>> HouseActors;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	float FieldElementSize = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TObjectPtr<class AHBVillage> VillageActor;
};