// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration/Dev/HBNoiseTest_Dev.h"

#include "ProceduralGeneration/MapData/HBMapDataGenerator.h"
#include "ProceduralGeneration/Noise/HBPerlinNoise.h"

AHBNoiseTest_Dev::AHBNoiseTest_Dev()
{
	
}

void AHBNoiseTest_Dev::BeginPlay()
{
	Super::BeginPlay();

	MapDataGenerateTest();
	MapDataTest();
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
	UE_LOG(LogTemp, Log, TEXT("[MapData] : \n%s"), *MapData.MapAs1D);
}

