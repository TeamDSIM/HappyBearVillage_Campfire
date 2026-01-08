// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralGeneration/MapData/HBMapData.h"
#include "HBVillage.generated.h"

struct FHBMapData;

UCLASS()
class HAPPYBEARVILLAGE_API AHBVillage : public AActor
{
	GENERATED_BODY()
	
public:
	AHBVillage();

	void SetMapData(const FHBMapData& InMapData);
	void ApplyVillageLocation(const FHBMapData& InMapData);
	void ApplyVillageSize(const FHBMapData& InMapData);
	void SpawnBlockingVolumes(const FHBMapData& InMapData);

	TArray<FVector> GetAvailableBlockLocations();
	
private:
	UPROPERTY(VisibleAnywhere, Category = "MapData", meta = (AllowPrivateAccess = "true"))
	FHBMapData MapData;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UArrowComponent> Arrow;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Village", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> FieldMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Village", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> BoundaryColliders;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Village", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class AHBBlockingVolume>> BlockingVolumes;
};
