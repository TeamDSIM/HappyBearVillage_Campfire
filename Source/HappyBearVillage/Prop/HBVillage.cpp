// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/HBVillage.h"

#include "Components/BoxComponent.h"
#include "ProceduralGeneration/MapData/HBMapData.h"

AHBVillage::AHBVillage()
{
	FieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FieldMesh"));
	RootComponent = FieldMesh;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshRef(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMeshRef.Succeeded())
	{
		FieldMesh->SetStaticMesh(CubeMeshRef.Object);
	}

	int32 DX[4] = {0, 0, -1, 1};
	int32 DY[4] = {-1, 1, 0, 0};
	FString BoundaryColliderNames[4] = {TEXT("BoundaryCollider_Top"), TEXT("BoundaryCollider_Bottom"), TEXT("BoundaryCollider_Left"), TEXT("BoundaryCollider_Right")};

	for (int32 Index = 0; Index<4; ++Index)
	{
		UBoxComponent* BoundaryCollider = CreateDefaultSubobject<UBoxComponent>(*BoundaryColliderNames[Index]);
		BoundaryCollider->SetupAttachment(RootComponent);
		BoundaryCollider->SetBoxExtent(FVector(50.f, 50.f, 150.f));
		BoundaryCollider->SetRelativeLocation(FVector(DX[Index], DY[Index], 1) * 100.f);
		BoundaryCollider->SetCollisionProfileName(TEXT("BlockAll"));
		
		BoundaryColliders.Add(BoundaryCollider);
	}
}

void AHBVillage::ApplyVillageLocation(const FHBMapData& InMapData)
{
	FVector Center = FVector(InMapData.Resolution.X, InMapData.Resolution.Y, 0) * InMapData.FieldElementScale * 0.5f;
	SetActorLocation(Center);
}

void AHBVillage::ApplyVillageSize(const FHBMapData& InMapData)
{
	FVector ActorScale = FVector(InMapData.Resolution.X, InMapData.Resolution.Y, 0) * InMapData.FieldElementScale;
	SetActorScale3D(ActorScale);
}
