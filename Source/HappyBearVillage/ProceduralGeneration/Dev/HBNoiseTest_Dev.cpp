// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration/Dev/HBNoiseTest_Dev.h"

#include "ProceduralGeneration/MapData/HBMapDataGenerator.h"
#include "ProceduralGeneration/MapData/HBMapGenerator.h"
#include "ProceduralGeneration/Noise/HBPerlinNoise.h"
#include "ProceduralGeneration/MapData/HBMapGenerator.h"

AHBNoiseTest_Dev::AHBNoiseTest_Dev()
{
	
}

void AHBNoiseTest_Dev::BeginPlay()
{
	Super::BeginPlay();

	MapGenerateTest();
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

void AHBNoiseTest_Dev::MapDataGenerateTest()
{
	UHBMapDataGenerator* Generator = NewObject<UHBMapDataGenerator>();

	FHBNoiseSettings Settings;
	Settings.Resolution = {64, 64};
	Settings.GridSize = {4, 4};
	Settings.Seed = Seed;
	
	Generator->GenerateMapData(Settings);
	Generator->PrintMapData();
}

void AHBNoiseTest_Dev::MapDataTest()
{
	UHBMapDataGenerator* Generator = NewObject<UHBMapDataGenerator>();

	FHBNoiseSettings Settings;
	Settings.Resolution = {64, 64};
	Settings.GridSize = {4, 4};
	Settings.Seed = Seed;
	
	FHBMapData MapData = Generator->GenerateMapData(Settings);
	UE_LOG(LogTemp, Log, TEXT("[String Size : %d]"), MapData.MapAs1D.Len());
	UE_LOG(LogTemp, Log, TEXT("[MapData] : \n%s"), *MapData.MapAs1D);
}

void AHBNoiseTest_Dev::MapGenerateTest()
{
	UHBMapDataGenerator* Generator = NewObject<UHBMapDataGenerator>();
	UHBMapGenerator* MapGenerator = NewObject<UHBMapGenerator>();
	
	FHBNoiseSettings Settings;
	Settings.Resolution = {64, 64};
	Settings.GridSize = {4, 4};
	Settings.Seed = Seed;
	
	FHBMapData MapData = Generator->GenerateMapData(Settings);
	MapGenerator->GenerateField(MapData, GetWorld());
}