// Fill out your copyright notice in the Description page of Project Settings.


#include "HBRoadField.h"
#include "Materials/MaterialInterface.h"


AHBRoadField::AHBRoadField()
{
	CubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMeshComponent;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		CubeMeshComponent->SetStaticMesh(CubeMeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FieldMaterialRef(TEXT("/Game/Assets/Fab/Ground/Materials/MI_xdhhdgq.MI_xdhhdgq"));
	if (FieldMaterialRef.Succeeded())
	{
		CubeMeshComponent->SetMaterial(0, FieldMaterialRef.Object);
	}
}