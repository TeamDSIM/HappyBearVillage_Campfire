// Fill out your copyright notice in the Description page of Project Settings.


#include "HBRoadField.h"


AHBRoadField::AHBRoadField()
{
	CubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMeshComponent;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		CubeMeshComponent->SetStaticMesh(CubeMeshAsset.Object);
	}
}