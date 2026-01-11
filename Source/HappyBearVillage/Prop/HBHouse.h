// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "HBHouse.generated.h"

class AHBCharacterPlayer;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterEnter, AActor* /*OtherActor*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterExit, AActor* /*OtherActor*/);

UCLASS()
class HAPPYBEARVILLAGE_API AHBHouse : public AActor
{
	GENERATED_BODY()
	
public:
	AHBHouse();

	FORCEINLINE FLinearColor GetHouseColor() const { return HouseColor; }
	FORCEINLINE void SetHouseColor(FLinearColor InHouseColor) { HouseColor = InHouseColor; }

	FORCEINLINE TArray<AHBCharacterPlayer*> GetOverlapCharacters() const { return OverlapCharacters; }
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEnterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnExitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Enter/ExitOverlap 에서 호출할 이벤트 델리게이트
	FOnCharacterEnter OnCharacterEnter;
	FOnCharacterExit OnCharacterExit;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UArrowComponent> Arrow;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> HouseMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> EnterTrigger;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> GateCollision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UChildActorComponent> HoneyPot;

	// 집 색깔
	UPROPERTY(VisibleAnywhere, Category = "House Info")
	FLinearColor HouseColor;

	// 집에 있는 사람 리스트
	UPROPERTY(VisibleAnywhere, Category = "House Info")
	TArray<AHBCharacterPlayer*> OverlapCharacters;
};
