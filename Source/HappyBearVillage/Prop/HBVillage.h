// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBVillage.generated.h"

struct FHBMapData;

UCLASS()
class HAPPYBEARVILLAGE_API AHBVillage : public AActor
{
	GENERATED_BODY()
	
public:
	AHBVillage();
	void ApplyVillageLocation(const FHBMapData& InMapData);
	void ApplyVillageSize(const FHBMapData& InMapData);
	void SpawnBlockingVolumes(const FHBMapData& InMapData);
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UArrowComponent> Arrow;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Village", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> FieldMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Village", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> BoundaryColliders;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Village", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class AHBBlockingVolume>> BlockingVolumes;
};
