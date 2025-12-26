// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBPCGVillageActor.generated.h"

UCLASS()
class HAPPYBEARVILLAGE_API AHBPCGVillageActor : public AActor
{
	GENERATED_BODY()

public:
	AHBPCGVillageActor();

	void InitializePCGInput(UTexture2D* Texture2D);
	void SetPCGBounds();
	void Generate();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UPCGComponent> PCG;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UBoxComponent> Bounds;
};
