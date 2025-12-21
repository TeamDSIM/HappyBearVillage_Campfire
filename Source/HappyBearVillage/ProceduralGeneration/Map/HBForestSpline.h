// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBForestSpline.generated.h"

UCLASS()
class HAPPYBEARVILLAGE_API AHBForestSpline : public AActor
{
	GENERATED_BODY()

public:
	AHBForestSpline();
 
	void Initialize(TArray<FVector> CurForestGridIndices, float FieldElemSize);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spline, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USplineComponent> ForestSpline;
};
