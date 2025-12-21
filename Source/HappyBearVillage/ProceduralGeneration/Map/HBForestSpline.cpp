// Fill out your copyright notice in the Description page of Project Settings.


#include "HBForestSpline.h"
#include "Components/SplineComponent.h"
#include "ProceduralGeneration/MapData/HBMapDataGenerator.h"


AHBForestSpline::AHBForestSpline()
{
	ForestSpline = CreateDefaultSubobject<USplineComponent>(TEXT("ForestField"));
	ForestSpline->bDrawDebug = true;
	RootComponent = ForestSpline;
}

void AHBForestSpline::Initialize(TArray<FVector> CurForestGridIndices, float FieldElementSize)
{
	if (CurForestGridIndices.Num() <= 1) return;

	ForestSpline->ClearSplinePoints(false);

	for (int32 i=0; i<CurForestGridIndices.Num(); ++i)
	{
		FVector Location = FVector(CurForestGridIndices[i].Y * FieldElementSize, CurForestGridIndices[i].X * FieldElementSize, FieldElementSize / 2);
		
		FSplinePoint SplinePoint(i, Location, ESplinePointType::Curve);
		ForestSpline->AddPoint(SplinePoint, false);
	}

	ForestSpline->SetClosedLoop(true, false);
	ForestSpline->UpdateSpline();
}