// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralGeneration/MapData/HBMapData.h"
#include "ProceduralGeneration/Noise/HBNoiseSettings.h"
#include "UObject/NoExportTypes.h"
#include "HBMapDataGenerator.generated.h"

/**
 * 
 */

constexpr int32 UNDEFINED = -100;
constexpr int32 MAX_WIDTH = 256;
constexpr int32 MAX_HEIGHT = 256;
constexpr int32 MAX_WEIGHT = 10000;

struct FMapNode
{
	int32 Row;
	int32 Col;

	int32 Perlin = UNDEFINED;
	int32 AreaID = UNDEFINED;
	TCHAR Type = ' ';
};

struct FMapEdge
{
	FMapNode* Src;
	FMapNode* Dest;
	int32 Weight = MAX_WEIGHT;
};

UCLASS()
class HAPPYBEARVILLAGE_API UHBMapDataGenerator : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE FHBMapData GetMapData() const { return MapData; }
	
	FHBMapData GenerateMapData(FHBNoiseSettings Settings);
	FHBMapData GenerateFieldData(class UHBPerlinNoise* InPerlinNoise);
	FHBMapData GenerateHouseData(int32 HouseCount);
	FHBMapData GenerateForestData();
	FHBMapData UpdateMap();

	UTexture2D* GenerateForestTexture2D();
	
	void PrintMapData();

private:
	void Union(int32 ID1, int32 ID2, TArray<int32>& Parents);
	int32 Find(int32 ID, TArray<int32>& Parents);
	bool IsForestBorder(FMapNode* Node);

private:
	int32 Width = 0;
	int32 Height = 0;
	int32 AreaCount = 0;
	int32 ForestCount = 0;

	UPROPERTY()
	TObjectPtr<UHBPerlinNoise> PerlinNoise;
	UPROPERTY()
	TObjectPtr<UTexture2D> ForestTexture2D;

	FMapNode Nodes[256][256];
	FMapEdge AreaAdj[256][256];

	TArray<FMapNode*> AreaBorderNodes;
	TArray<TArray<FVector>> ForestBorderGridIndices;

	FHBMapData MapData;
};
