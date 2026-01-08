#include "HBVillage.h"

#include "HBBlockingVolume.h"
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
		BoundaryCollider->SetupAttachment(RootComponent);

		FVector VerticalExtent = FVector(50.f, 0.f, 500.f);
		FVector HorizontalExtent = FVector(0.f, 50.f, 500.f);
		FVector Extent = (Index < 2) ? VerticalExtent : HorizontalExtent;
		
		BoundaryCollider->SetBoxExtent(Extent);
		BoundaryCollider->SetRelativeLocation(FVector(DX[Index] * 50.f, DY[Index] * 50.f, 500.f - 50.f));
		BoundaryCollider->SetCollisionProfileName("BlockAllDynamic");
      
		BoundaryColliders.Add(BoundaryCollider);
	}
}

void AHBVillage::SetMapData(const FHBMapData& InMapData)
{
	MapData = InMapData;
}

void AHBVillage::ApplyVillageLocation(const FHBMapData& InMapData)
{
	const FVector FirstCenter = FVector(0, 0, 0) * InMapData.AreaScale * 100;
	const FVector LastCenter = FVector(InMapData.Resolution.X - 1, InMapData.Resolution.Y - 1, 0) * InMapData.AreaScale * 100;
	
	const FVector VillageCenter = (FirstCenter + LastCenter) * 0.5f;
	SetActorLocation(VillageCenter);

	int32 DX[4] = {0, 0, -1, 1};
	int32 DY[4] = {-1, 1, 0, 0};
	
	for (int32 Index = 0; Index<4; ++Index)
	{
		UBoxComponent* BoundaryCollider = BoundaryColliders[Index];

		FVector VerticalExtent = FVector(50.f * InMapData.Resolution.X * InMapData.AreaScale, 0.f, 500.f);
		FVector HorizontalExtent = FVector(0.f, 50.f * InMapData.Resolution.Y * InMapData.AreaScale, 500.f);
		FVector Extent = (Index < 2) ? VerticalExtent : HorizontalExtent;
		
		BoundaryCollider->SetBoxExtent(Extent);
		BoundaryCollider->SetRelativeLocation(FVector(DX[Index] * 50.f * InMapData.Resolution.X * InMapData.AreaScale, DY[Index] * 50.f * InMapData.Resolution.Y * InMapData.AreaScale, 500.f - 50.f));
      
		BoundaryColliders.Add(BoundaryCollider);
	}
}

void AHBVillage::ApplyVillageSize(const FHBMapData& InMapData)
{
	FVector MeshScale = FVector(InMapData.Resolution.X * InMapData.AreaScale, InMapData.Resolution.Y * InMapData.AreaScale, 1);
	FieldMesh->SetRelativeScale3D(MeshScale);
}

void AHBVillage::SpawnBlockingVolumes(const FHBMapData& InMapData)
{
	for (int32 Row = 0; Row < InMapData.Resolution.Y; ++Row)
	{
		for (int32 Col = 0; Col < InMapData.Resolution.X; ++Col)
		{
			TCHAR TileType = InMapData.Map[Row][Col];

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(FVector(Col * InMapData.AreaScale * 100.f, Row * InMapData.AreaScale * 100.f, 0));
			SpawnTransform.SetRotation(FQuat(FRotator::ZeroRotator));
			SpawnTransform.SetScale3D(FVector::OneVector * InMapData.AreaScale);
			
			if (TileType == ' ' || TileType == 'F')
			{
				AHBBlockingVolume* BlockingVolume = GetWorld()->SpawnActor<AHBBlockingVolume>(AHBBlockingVolume::StaticClass(), SpawnTransform);
				BlockingVolumes.Add(BlockingVolume);
			}
		}
	}
}

TArray<FVector> AHBVillage::GetAvailableBlockLocations()
{
	TArray<FVector> AvailableLocations;

	for (int32 Row = 0; Row < MapData.Resolution.Y; ++Row)
	{
		for (int32 Col = 0; Col < MapData.Resolution.X; ++Col)
		{
			TCHAR TileType = MapData.Map[Row][Col];
			if (TileType == 'A' || TileType == 'R')
			{
				AvailableLocations.Add(FVector(Col * MapData.AreaScale * 100.f, Row * MapData.AreaScale * 100.f, 0));
			}
		}
	}

	return AvailableLocations;
}
