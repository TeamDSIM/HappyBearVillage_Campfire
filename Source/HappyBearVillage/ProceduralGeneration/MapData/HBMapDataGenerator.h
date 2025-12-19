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

struct FMapNode
{
	int32 Row;
	int32 Col;

	int32 Perlin = -100;
	int32 AreaID = -1;
	TCHAR Type = ' ';
};

struct FMapEdge
{
	FMapNode* Src;
	FMapNode* Dest;
	int32 Weight = 10000;
};

UCLASS()
class HAPPYBEARVILLAGE_API UHBMapDataGenerator : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE FHBMapData GetMapData() const { return Result; }
	
	FHBMapData GenerateMapData(class UHBPerlinNoise* InPerlinNoise);
	FHBMapData GenerateMapData(FHBNoiseSettings Settings);
	FHBMapData AddHouseInfo(int32 HouseCount);
	void PrintMapData();

private:
	void Union(int32 ID1, int32 ID2, TArray<int32>& Parents);
	int32 Find(int32 ID, TArray<int32>& Parents);

private:
	int32 Width = 0;
	int32 Height = 0;
	int32 AreaCount = 0;

	FMapNode Nodes[256][256];
	FMapEdge AreaAdj[256][256];

	TArray<FMapNode*> BorderNodes;

	FHBMapData Result;
};
