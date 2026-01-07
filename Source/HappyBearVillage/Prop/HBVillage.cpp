#include "HBVillage.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ProceduralGeneration/MapData/HBMapData.h"

AHBVillage::AHBVillage()
{
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	RootComponent = Arrow;
	
	FieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FieldMesh"));
	FieldMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		FieldMesh->SetStaticMesh(CubeMesh.Object);
		FieldMesh->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
	}

	int32 DX[4] = {0, 0, -1, 1};
	int32 DY[4] = {-1, 1, 0, 0};
	FString BoundaryColliderNames[4] = {TEXT("BoundaryCollider_Top"), TEXT("BoundaryCollider_Bottom"), TEXT("BoundaryCollider_Left"), TEXT("BoundaryCollider_Right")};
	
	for (int32 Index = 0; Index<4; ++Index)
	{
		UBoxComponent* BoundaryCollider = CreateDefaultSubobject<UBoxComponent>(*BoundaryColliderNames[Index]);
		BoundaryCollider->SetupAttachment(FieldMesh);

		FVector Extent = (Index < 2) ? FVector(50.f, 50.f, 500.f) : FVector(50.f, 50.f, 500.f);
		BoundaryCollider->SetBoxExtent(Extent);
		BoundaryCollider->SetRelativeLocation(FVector(DX[Index] * 100.f, DY[Index] * 100.f, 250.f - 50.f));
		BoundaryCollider->SetCollisionProfileName(TEXT("BlockAll"));
      
		BoundaryColliders.Add(BoundaryCollider);
	}
}

void AHBVillage::ApplyVillageLocation(const FHBMapData& InMapData)
{
	const FVector Center = FVector(InMapData.Resolution.X - 1, InMapData.Resolution.Y - 1, 0) * InMapData.AreaScale * 100 * 0.5f;
	SetActorLocation(Center);
}

void AHBVillage::ApplyVillageSize(const FHBMapData& InMapData)
{
	FVector ActorScale = FVector(InMapData.Resolution.X * InMapData.AreaScale, InMapData.Resolution.Y * InMapData.AreaScale, 1);
	SetActorScale3D(ActorScale);
}