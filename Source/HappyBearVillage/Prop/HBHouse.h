// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "HBHouse.generated.h"

UCLASS()
class HAPPYBEARVILLAGE_API AHBHouse : public AActor
{
	GENERATED_BODY()
	
public:
	AHBHouse();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEnterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnExitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UArrowComponent> Arrow;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> HouseMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> EnterTrigger;
};
