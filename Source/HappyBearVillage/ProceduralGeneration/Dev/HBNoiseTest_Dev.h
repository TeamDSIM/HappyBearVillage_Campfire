// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBNoiseTest_Dev.generated.h"

UCLASS()
class HAPPYBEARVILLAGE_API AHBNoiseTest_Dev : public AActor
{
	GENERATED_BODY()
	
public:	
	AHBNoiseTest_Dev();

protected:
	virtual void BeginPlay() override;
	void NoiseTest();
	void MapDataGenerateTest();
	void MapDataTest();
	void MapGenerateTest();

public:
	UPROPERTY(EditAnywhere)
	float Seed = 0.125f;
};
