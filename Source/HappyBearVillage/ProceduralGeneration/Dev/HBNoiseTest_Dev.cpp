// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration/Dev/HBNoiseTest_Dev.h"
#include "ProceduralGeneration/Noise/HBPerlinNoise.h"

AHBNoiseTest_Dev::AHBNoiseTest_Dev()
{
	
}

void AHBNoiseTest_Dev::BeginPlay()
{
	Super::BeginPlay();

	PerlinNoise = NewObject<UHBPerlinNoise>();

	FHBNoiseSettings Settings;
	Settings.Resolution = {64, 64};
	Settings.GridSize = {4, 4};
	Settings.Seed = Seed;
	
	PerlinNoise->GeneratePerlinNoise(Settings);
	PerlinNoise->PrintNoise();
}

