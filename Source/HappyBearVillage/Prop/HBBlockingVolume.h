// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralGeneration/MapData/HBMapData.h"
#include "HBBlockingVolume.generated.h"

class UBoxComponent;

UCLASS()
class HAPPYBEARVILLAGE_API AHBBlockingVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AHBBlockingVolume();

private:
	UPROPERTY(VisibleAnywhere, Category = "Volume")
	TObjectPtr<UBoxComponent> BoxComponent;
};
