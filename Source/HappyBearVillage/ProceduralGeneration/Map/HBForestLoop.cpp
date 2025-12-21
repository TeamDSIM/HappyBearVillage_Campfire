// Fill out your copyright notice in the Description page of Project Settings.


#include "HBForestLoop.h"
#include "Components/SplineComponent.h"
#include "ProceduralGeneration/MapData/HBMapDataGenerator.h"


AHBForestLoop::AHBForestLoop()
{
	ForestField = CreateDefaultSubobject<USplineComponent>(TEXT("ForestField"));
	ForestField->bDrawDebug = true;
	RootComponent = ForestField;
}

void AHBForestLoop::Initialize(TArray<FVector> CurForestGridIndices, float FieldElementSize)
{
	if (CurForestGridIndices.Num() <= 1) return;

	ForestField->ClearSplinePoints(false);

	for (int32 i=0; i<CurForestGridIndices.Num(); ++i)
	{
		FVector Location = FVector(CurForestGridIndices[i].Y * FieldElementSize, CurForestGridIndices[i].X * FieldElementSize, FieldElementSize / 2);
		
		FSplinePoint SplinePoint(i, Location, ESplinePointType::Curve);
		ForestField->AddPoint(SplinePoint, false);
	}

	ForestField->SetClosedLoop(true, false);
	ForestField->UpdateSpline();
}