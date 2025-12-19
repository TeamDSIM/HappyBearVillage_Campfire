// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration/MapData/HBMapGenerator.h"
#include "Engine/World.h"
#include "ProceduralGeneration/Map/HBForestField.h"
#include "ProceduralGeneration/Map/HBRoadField.h"

UHBMapGenerator::UHBMapGenerator()
{
	RoadFieldClass = AHBRoadField::StaticClass();
	ForestFieldClass = AHBForestField::StaticClass();
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
			
			if (TileType == 'A' || TileType == 'R')
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