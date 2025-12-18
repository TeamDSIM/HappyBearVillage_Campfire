// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBRoadField.generated.h"

UCLASS()
class HAPPYBEARVILLAGE_API AHBRoadField : public AActor
{
	GENERATED_BODY()

public:
	AHBRoadField();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> CubeMeshComponent;
};
