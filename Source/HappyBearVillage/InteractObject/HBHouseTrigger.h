// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBHouseTrigger.generated.h"

class UBoxComponent;

UCLASS()
class HAPPYBEARVILLAGE_API AHBHouseTrigger : public AActor
{
    GENERATED_BODY()

public:
    AHBHouseTrigger();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Trigger")
    TObjectPtr<UBoxComponent> TriggerBox;

    UFUNCTION()
    void OnBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    UFUNCTION()
    void OnEndOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
    );
};
