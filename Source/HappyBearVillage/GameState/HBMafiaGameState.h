// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HBMafiaGameState.generated.h"

/**
 * 
 */

// 각 페이즈를 나타내는 enum 클래스
UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	Day,	// 대기 상태 (낮)
	Discussion,	// 토론 상태
	Fight,		// 난투 상태
	Vote,		// 최종 투표 상태
	Night,		// 밤 상태

	End,
};


UCLASS()
class HAPPYBEARVILLAGE_API AHBMafiaGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AHBMafiaGameState();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// @PHYTODO : 나중에 다 컴포넌트로 생성해서 옮겨주기
	UPROPERTY(ReplicatedUsing = OnRep_GamePhase, BlueprintReadOnly)
	EGamePhase CurrentPhase = EGamePhase::Day;

	UPROPERTY(ReplicatedUsing = OnRep_RemainingTime, BlueprintReadOnly)
	float RemainingTime = 0.f;

	UFUNCTION()
	void OnRep_GamePhase();

	UFUNCTION()
	void OnRep_RemainingTime();
};
