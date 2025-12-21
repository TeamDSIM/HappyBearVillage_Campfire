// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBForestLoop.generated.h"

UCLASS()
class HAPPYBEARVILLAGE_API AHBForestLoop : public AActor
{
	GENERATED_BODY()

public:
	AHBForestLoop();
 
	void Initialize(TArray<FVector> CurForestGridIndices, float FieldElemSize);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spline, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USplineComponent> ForestField;
};
