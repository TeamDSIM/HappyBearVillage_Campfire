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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> FieldMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collider")
	TArray<TObjectPtr<class UBoxComponent>> BoundaryColliders;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collider")
	TArray<TObjectPtr<class UBoxComponent>> BlockingColliders;
};
