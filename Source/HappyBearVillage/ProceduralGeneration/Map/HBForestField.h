// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBForestField.generated.h"

UCLASS()
class HAPPYBEARVILLAGE_API AHBForestField : public AActor
{
	GENERATED_BODY()

public:
	AHBForestField();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> BaseCubeMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> SecondCubeMeshComponent;
};
