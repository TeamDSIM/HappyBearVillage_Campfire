// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "HBExecutePlatform.generated.h"


UCLASS()
class HAPPYBEARVILLAGE_API AHBExecutePlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHBExecutePlatform();

public:
	FORCEINLINE FVector GetTargetPosition() const { return TargetPosition.Get()->GetComponentLocation(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> PlatformMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> TargetPosition;
};
