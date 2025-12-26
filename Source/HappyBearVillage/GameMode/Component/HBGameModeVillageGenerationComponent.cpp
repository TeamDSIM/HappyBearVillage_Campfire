// Fill out your copyright notice in the Description page of Project Settings.


#include "HBGameModeVillageGenerationComponent.h"

#include "GameState/HBMafiaGameState.h"
#include "ProceduralGeneration/Map/HBMapGenerator.h"
#include "ProceduralGeneration/MapData/HBMapDataGenerator.h"
#include "ProceduralGeneration/MapData/HBMapSyncInfo.h"
#include "ProceduralGeneration/Noise/HBPerlinNoise.h"
#include "Utils/HBUtils.h"

UHBGameModeVillageGenerationComponent::UHBGameModeVillageGenerationComponent()
{

}

void UHBGameModeVillageGenerationComponent::SyncVillage(int32 PlayerCount)
{
	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (!HBGameState)
	{
		return;
	}
	
	int32 RandomStreamSeed = FMath::RandRange(0, 10000);
	HBUtils::InitRandomSeed(RandomStreamSeed);

	FHBNoiseSettings NoiseSettings;
	NoiseSettings.Resolution = {64, 64};
	NoiseSettings.GridSize = {4, 4};
	NoiseSettings.Seed = FMath::RandRange(0.0f, 10000.0f);

	FHBVillageGenerationSyncData VillageGenerationSyncData;
	VillageGenerationSyncData.RandomStreamSeed = RandomStreamSeed;
	VillageGenerationSyncData.NoiseSettings = NoiseSettings;

	HBGameState->VillageGenerationSyncData = VillageGenerationSyncData;
	HBGameState->OnRep_VillageGenerationSyncData();
}
