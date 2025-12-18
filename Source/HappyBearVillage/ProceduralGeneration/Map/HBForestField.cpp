// Fill out your copyright notice in the Description page of Project Settings.


#include "HBForestField.h"


AHBForestField::AHBForestField()
{
	BaseCubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseCubeMesh"));
    RootComponent = BaseCubeMeshComponent;
    
    SecondCubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondCubeMesh"));
    SecondCubeMeshComponent->SetupAttachment(RootComponent);
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMeshAsset.Succeeded())
    {
    	BaseCubeMeshComponent->SetStaticMesh(CubeMeshAsset.Object);
    	SecondCubeMeshComponent->SetStaticMesh(CubeMeshAsset.Object);
    }

    SecondCubeMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
}