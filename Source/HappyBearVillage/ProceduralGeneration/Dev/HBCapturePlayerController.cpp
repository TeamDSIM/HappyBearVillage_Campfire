// Fill out your copyright notice in the Description page of Project Settings.


#include "HBCapturePlayerController.h"

#include <Subsystem/HBVillageGenerationWorldSubsystem.h>

void AHBCapturePlayerController::GenerateVillage()
{
	UHBVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UHBVillageGenerationWorldSubsystem>();
	if (!VillageGenerationSystem) return;

	int32 RandomStreamSeed = FMath::RandRange(0, 10000);

	FHBNoiseSettings NoiseSettings;
	NoiseSettings.Resolution = {64, 64};
	NoiseSettings.GridSize = {4, 4};
	NoiseSettings.Seed = FMath::RandRange(0.0f, 10000.0f);

	TArray<FLinearColor> HouseColorList;

	FHBVillageGenerationData VillageGenerationSyncData;
	VillageGenerationSyncData.RandomStreamSeed = RandomStreamSeed;
	VillageGenerationSyncData.NoiseSettings = NoiseSettings;
	VillageGenerationSyncData.HouseColorList = HouseColorList;

	VillageGenerationSyncData.HouseColorList.Add(FLinearColor(0, 0, 0, 1));
	VillageGenerationSyncData.HouseColorList.Add(FLinearColor(0, 0, 1, 1));
	VillageGenerationSyncData.HouseColorList.Add(FLinearColor(0, 1, 0, 1));
	VillageGenerationSyncData.HouseColorList.Add(FLinearColor(0, 1, 1, 1));
	
	VillageGenerationSyncData.HouseColorList.Add(FLinearColor(1, 0, 0, 1));
	VillageGenerationSyncData.HouseColorList.Add(FLinearColor(1, 0, 1, 1));
	VillageGenerationSyncData.HouseColorList.Add(FLinearColor(1, 1, 0, 1));
	VillageGenerationSyncData.HouseColorList.Add(FLinearColor(1, 1, 1, 1));
	
	VillageGenerationSystem->GenerateVillage(VillageGenerationSyncData);
}
