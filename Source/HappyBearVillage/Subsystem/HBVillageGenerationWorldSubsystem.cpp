// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/HBVillageGenerationWorldSubsystem.h"

#include "ProceduralGeneration/Map/HBMapGenerator.h"
#include "ProceduralGeneration/MapData/HBMapDataGenerator.h"
#include "ProceduralGeneration/Noise/HBPerlinNoise.h"
#include "Utils/HBUtils.h"

void UHBVillageGenerationWorldSubsystem::GenerateVillage(FHBVillageGenerationData InVillageGenerationData)
{
	HBUtils::InitRandomSeed(InVillageGenerationData.RandomStreamSeed);

	PerlinNoise = NewObject<UHBPerlinNoise>();
	MapDataGenerator = NewObject<UHBMapDataGenerator>();
	MapGenerator = NewObject<UHBMapGenerator>();

	PerlinNoise->GeneratePerlinNoise(InVillageGenerationData.NoiseSettings);

	MapDataGenerator->GenerateFieldData(PerlinNoise);
	MapDataGenerator->GenerateHouseData(InVillageGenerationData.HouseColorList.Num());
	MapDataGenerator->GenerateHouseColorData(InVillageGenerationData.HouseColorList);
	MapDataGenerator->GenerateForestData();
	MapDataGenerator->GenerateForestTexture2D();
	MapDataGenerator->UpdateMap();

	FHBMapData MapData = MapDataGenerator->GetMapData();

	MapGenerator->GenerateField(MapData, GetWorld());
	MapGenerator->GenerateHouse(MapData, GetWorld());
	MapGenerator->GenerateForestSpline(MapData, GetWorld());
	MapGenerator->GenerateVillage(MapData, GetWorld());

	bIsGenerated = true;

	OnVillageGenerated.Broadcast();
}
