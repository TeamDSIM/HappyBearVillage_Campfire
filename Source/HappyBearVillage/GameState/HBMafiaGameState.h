// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Subsystem/HBGameFlowSubsystem.h"
#include "HBMafiaGameState.generated.h"

/**
 * 
 */
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
	EGamePhase CurrentPhase = EGamePhase::Waiting;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float RemainingTime = 0.f;

	UFUNCTION()
	void OnRep_GamePhase();
};
