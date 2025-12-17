// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HBCharacterStat.h"
#include "Components/ActorComponent.h"
#include "HBPlayerStatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HAPPYBEARVILLAGE_API UHBPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHBPlayerStatComponent();

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:
	FORCEINLINE const FHBCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE float GetTotalTakenDamage() const { return TotalTakenDamage; }
	FORCEINLINE bool GetIsVoteTarget() const { return bIsVoteTarget; }
	FORCEINLINE bool GetIsAlive() const { return bIsAlive; }

	float ApplyDamage(float InDamageAmount);

protected:
	// 캐릭터 기본 스탯
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	FHBCharacterStat BaseStat;

	// 투표 시 캐릭터가 받은 데미지
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float TotalTakenDamage = 0.f;

	// 캐릭터 축출 대상 여부
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	uint8 bIsVoteTarget : 1;

	// 캐릭터 생존 여부
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	uint8 bIsAlive : 1;

	// 
};
