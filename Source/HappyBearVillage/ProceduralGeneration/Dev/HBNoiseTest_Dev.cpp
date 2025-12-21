// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration/Dev/HBNoiseTest_Dev.h"

#include "ProceduralGeneration/Noise/HBPerlinNoise.h"
#include "ProceduralGeneration/MapData/HBMapDataGenerator.h"
#include "ProceduralGeneration/Map/HBMapGenerator.h"

AHBNoiseTest_Dev::AHBNoiseTest_Dev()
{
	
}

void AHBNoiseTest_Dev::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateForestTest();
}

void AHBNoiseTest_Dev::NoiseTest()
{
	UHBPerlinNoise* PerlinNoise = NewObject<UHBPerlinNoise>();

	FHBNoiseSettings Settings;
	Settings.Resolution = {64, 64};
	Settings.GridSize = {4, 4};
	Settings.Seed = Seed;
	
	PerlinNoise->GeneratePerlinNoise(Settings);
	PerlinNoise->PrintNoise();
}

void AHBNoiseTest_Dev::GenerateFieldDataTest()
{
	UHBPerlinNoise* PerlinNoise = NewObject<UHBPerlinNoise>();
	UHBMapDataGenerator* MapDataGenerator = NewObject<UHBMapDataGenerator>();

	FHBNoiseSettings Settings;
	Settings.Resolution = {64, 64};
	Settings.GridSize = {4, 4};
	Settings.Seed = Seed;

	PerlinNoise->GeneratePerlinNoise(Settings);
	
	MapDataGenerator->GenerateFieldData(PerlinNoise);
	MapDataGenerator->PrintMapData();
}

void AHBNoiseTest_Dev::GenerateFieldTest()
{
	UHBPerlinNoise* PerlinNoise = NewObject<UHBPerlinNoise>();
	UHBMapDataGenerator* MapDataGenerator = NewObject<UHBMapDataGenerator>();
	UHBMapGenerator* MapGenerator = NewObject<UHBMapGenerator>();
	
	FHBNoiseSettings Settings;
	Settings.Resolution = {64, 64};
	Settings.GridSize = {4, 4};
	Settings.Seed = Seed;

	PerlinNoise->GeneratePerlinNoise(Settings);
	FHBMapData MapData = MapDataGenerator->GenerateFieldData(PerlinNoise);
	MapGenerator->GenerateField(MapData, GetWorld());
}

void AHBNoiseTest_Dev::GenerateHouseTest()
{
	UHBPerlinNoise* PerlinNoise = NewObject<UHBPerlinNoise>();
	UHBMapDataGenerator* MapDataGenerator = NewObject<UHBMapDataGenerator>();
	UHBMapGenerator* MapGenerator = NewObject<UHBMapGenerator>();
	
	FHBNoiseSettings Settings;
	Settings.Resolution = {64, 64};
	Settings.GridSize = {4, 4};
	Settings.Seed = Seed;

	PerlinNoise->GeneratePerlinNoise(Settings);
	FHBMapData MapData = MapDataGenerator->GenerateFieldData(PerlinNoise);
	MapData = MapDataGenerator->GenerateHouseData(8);

	MapGenerator->GenerateField(MapData, GetWorld());
	MapGenerator->GenerateHouse(MapData, GetWorld());
}

void AHBNoiseTest_Dev::GenerateForestTest()
{
	UHBPerlinNoise* PerlinNoise = NewObject<UHBPerlinNoise>();
	UHBMapDataGenerator* MapDataGenerator = NewObject<UHBMapDataGenerator>();
	UHBMapGenerator* MapGenerator = NewObject<UHBMapGenerator>();
	
	FHBNoiseSettings Settings;
	Settings.Resolution = {64, 64};
	Settings.GridSize = {4, 4};
	Settings.Seed = Seed;

	PerlinNoise->GeneratePerlinNoise(Settings);
	FHBMapData MapData = MapDataGenerator->GenerateFieldData(PerlinNoise);
	MapData = MapDataGenerator->GenerateHouseData(8);
	MapData = MapDataGenerator->GenerateForestData();

	MapGenerator->GenerateField(MapData, GetWorld());
	MapGenerator->GenerateHouse(MapData, GetWorld());
	MapGenerator->GenerateForestSpline(MapData, GetWorld());

	MapDataGenerator->PrintMapData();
}

void AHBNoiseTest_Dev::MapDataTest()
{
	UHBMapDataGenerator* MapDataGenerator = NewObject<UHBMapDataGenerator>();

	FHBNoiseSettings Settings;
	Settings.Resolution = {64, 64};
	Settings.GridSize = {4, 4};
	Settings.Seed = Seed;
	
	FHBMapData MapData = MapDataGenerator->GenerateMapData(Settings);

	FString MapDataString;
	for (int32 Row = 0; Row < MapData.Resolution.Y; ++Row)
	{
		for (int32 Col = 0; Col < MapData.Resolution.X; ++Col)
		{
			MapDataString += MapData.Map[Row][Col];
		}
		MapDataString += TEXT("\n");
	}
	
	UE_LOG(LogTemp, Log, TEXT("[MapData] : \n%s"), *MapDataString);
}
