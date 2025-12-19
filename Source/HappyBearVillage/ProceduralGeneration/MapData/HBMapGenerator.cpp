// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration/MapData/HBMapGenerator.h"
#include "Engine/World.h"
#include "ProceduralGeneration/Map/HBForestField.h"
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

	if (Width <= 0 || Height <= 0 || MapData.MapAs1D.Len() != Width * Height)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid MapData"));
		return;
	}

	for (int32 Row = 0; Row < Height; ++Row)
	{
		for (int32 Col = 0; Col < Width; ++Col)
		{
			int32 Index = Row * Width + Col;
			TCHAR TileType = MapData.MapAs1D[Index];

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
				AActor* FieldActor = InWorld->SpawnActor<AHBForestField>(ForestFieldClass, SpawnTransform);
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

	if (Width <= 0 || Height <= 0 || MapData.MapAs1D.Len() != Width * Height)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid MapData"));
		return;
	}

	for (int32 Row = 0; Row < Height; ++Row)
	{
		for (int32 Col = 0; Col < Width; ++Col)
		{
			int32 Index = Row * Width + Col;
			TCHAR TileType = MapData.MapAs1D[Index];

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
