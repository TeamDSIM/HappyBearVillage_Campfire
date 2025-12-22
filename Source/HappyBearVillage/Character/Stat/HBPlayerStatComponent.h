// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HBCharacterRole.h"
#include "HBCharacterStat.h"
#include "Components/ActorComponent.h"
#include "HBPlayerStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnTotalTakenDamageChanged, float /*CurrentTotalDamage*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerRoleChanged, ERoleType)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerJobChanged, EJobType)

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
	FOnTotalTakenDamageChanged OnTotalTakenDamageChanged;
	FOnPlayerRoleChanged OnPlayerRoleChanged;
	FOnPlayerJobChanged OnPlayerJobChanged;

public:
	// 스탯 관련 섹션 =====================================================================
	FORCEINLINE const FHBCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE float GetTotalTakenDamage() const { return TotalTakenDamage; }
	FORCEINLINE bool GetIsVoteTarget() const { return bIsVoteTarget; }
	FORCEINLINE bool GetIsAlive() const { return bIsAlive; }

	float ApplyDamage(float InDamageAmount);

	// 직업 관련 섹션 =====================================================================
	FORCEINLINE const FHBCharacterRole& GetCharacterRole() const { return CharacterRole; }

	// 캐릭터 직업 부여
	void InitCharacterRole();

	// 캐릭터 직업 초기화
	void ResetCharacterRole();

protected:
	// 캐릭터 기본 스탯
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	FHBCharacterStat BaseStat;

	// 캐릭터 직업
	UPROPERTY(ReplicatedUsing = OnRep_CharacterRole, Transient, VisibleInstanceOnly, Category = Stat)
	FHBCharacterRole CharacterRole;

	// 투표 시 캐릭터가 받은 데미지
	UPROPERTY(ReplicatedUsing = OnRep_TotalTakenDamage, Transient, VisibleInstanceOnly, Category = Stat)
	float TotalTakenDamage = 0.f;

	// 캐릭터 축출 대상 여부
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	uint8 bIsVoteTarget : 1;

	// 캐릭터 생존 여부
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	uint8 bIsAlive : 1;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_TotalTakenDamage();

	UFUNCTION()
	void OnRep_CharacterRole();
};
