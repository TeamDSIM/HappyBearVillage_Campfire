// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration/Map/HBMapGenerator.h"
#include "Engine/World.h"
#include "ProceduralGeneration/Map/HBForestField.h"
#include "ProceduralGeneration/Map/HBForestSpline.h"
#include "ProceduralGeneration/Map/HBRoadField.h"

UHBMapGenerator::UHBMapGenerator()
{
	RoadFieldClass = AHBRoadField::StaticClass();
	ForestFieldClass = AHBForestField::StaticClass();

	static ConstructorHelpers::FClassFinder<AActor> OldHouseRef(TEXT("/Script/Engine.Blueprint'/Game/Map/BP_Old_House.BP_Old_House_C'"));
	if (OldHouseRef.Succeeded())
	{
		HouseClasses.Add(OldHouseRef.Class);
	}
	
	static ConstructorHelpers::FClassFinder<AActor> AbandonedHouseRef(TEXT("/Script/Engine.Blueprint'/Game/Map/BP_Abandoned_House.BP_Abandoned_House_C'"));
	if (AbandonedHouseRef.Succeeded())
	{
		HouseClasses.Add(AbandonedHouseRef.Class);
	}
	
	static ConstructorHelpers::FClassFinder<AActor> VikingHouseRef(TEXT("/Script/Engine.Blueprint'/Game/Map/BP_Viking_House.BP_Viking_House_C'"));
	if (VikingHouseRef.Succeeded())
	{
		HouseClasses.Add(VikingHouseRef.Class);
	}
}

void UHBMapGenerator::GenerateField(FHBMapData InMapData, UWorld* InWorld)
{
	if (!RoadFieldClass || !ForestFieldClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Generate Field Error"));
		return;
	}

	MapData = InMapData;

	const int32 Width = MapData.Resolution.X;
	const int32 Height = MapData.Resolution.Y;

	if (Width <= 0 || Height <= 0 || Height != MapData.Map.Num() || Width != MapData.Map[0].Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid MapData"));
		return;
	}

	for (int32 Row = 0; Row < Height; ++Row)
	{
		for (int32 Col = 0; Col < Width; ++Col)
		{
			TCHAR TileType = MapData.Map[Row][Col];

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(FVector(Col * FieldElementSize, Row * FieldElementSize, 0));
			SpawnTransform.SetRotation(FQuat(FRotator::ZeroRotator));
			SpawnTransform.SetScale3D(FVector::OneVector * (FieldElementSize / 100));
			
			FVector SpawnLocation = FVector(Col * FieldElementSize, Row * FieldElementSize, 0.0f);
			
			if (TileType != ' ')
			{
				AActor* FieldActor = InWorld->SpawnActor<AHBRoadField>(RoadFieldClass, SpawnTransform);
				FieldActors.Add(FieldActor);
			}
			else
			{
				//AActor* FieldActor = InWorld->SpawnActor<AHBForestField>(ForestFieldClass, SpawnTransform);
				//FieldActors.Add(FieldActor);
				AActor* FieldActor = InWorld->SpawnActor<AHBRoadField>(RoadFieldClass, SpawnTransform);
				FieldActors.Add(FieldActor);
			}
		}
	}
}

void UHBMapGenerator::GenerateHouse(FHBMapData InMapData, UWorld* InWorld)
{
	if (HouseClasses.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Generate House Error"));
		return;
	}

	MapData = InMapData;

	const int32 Width = MapData.Resolution.X;
	const int32 Height = MapData.Resolution.Y;

	if (Width <= 0 || Height <= 0 || Height != MapData.Map.Num() || Width != MapData.Map[0].Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid MapData"));
		return;
	}

	for (int32 Row = 0; Row < Height; ++Row)
	{
		for (int32 Col = 0; Col < Width; ++Col)
		{
			TCHAR TileType = MapData.Map[Row][Col];

			TSubclassOf<AActor> RandomClass = HouseClasses[FMath::RandRange(0, HouseClasses.Num() - 1)]; // ToDo : 동기화 과정에서 Rand 함수 대체 필요
			FVector SpawnLocation = FVector((Col + 1.5f) * FieldElementSize, (Row + 1.5f) * FieldElementSize, FieldElementSize / 2);
			
			if (TileType == 'H')
			{
				AActor* HouseActor = InWorld->SpawnActor<AActor>(RandomClass, SpawnLocation, FRotator::ZeroRotator);
				HouseActors.Add(HouseActor);
			}
		}
	}
}

void UHBMapGenerator::GenerateForestSpline(FHBMapData InMapData, UWorld* InWorld)
{
	MapData = InMapData;

	for (int i=1; i<InMapData.ForestBorderGridIndices.Num(); ++i)
	{
		TArray<FVector> CurForestBorderGridIndices = InMapData.ForestBorderGridIndices[i];

		AHBForestSpline* ForestLoop = InWorld->SpawnActor<AHBForestSpline>(AHBForestSpline::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		ForestLoop->Initialize(CurForestBorderGridIndices, FieldElementSize);
	}
}
