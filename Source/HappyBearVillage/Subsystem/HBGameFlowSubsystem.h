// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameState/HBMafiaGameState.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HBGameFlowSubsystem.generated.h"

/**
*	작성일: 2025-12-18
 *  작성자: 박현용
 *  설명: 마피아 게임 플로우를 관리하는 서브시스템
 */

UCLASS()
class HAPPYBEARVILLAGE_API UHBGameFlowSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 게임 시작 시 호출될 함수
	void StartGame();
	void StopGame();

private:
	// 각 페이즈 별 시작 함수
	void StartDay();		// 낮 (토론 전 잠시 대기)
	void StartDiscussion();		// 토론
	void StartFight();			// 난투
	void StartVote();			// 투표
	void StartNight();			// 밤
	
	// 페이즈 변환 함수
	void SetPhase(EGamePhase NewPhase, float Duration);

	// 시간초 감소 함수
	void StartCountdown(float StartTime);
	void TickCountdown();
	
	
	// 각 페이즈 별 시간 관리할 타이머
	FTimerHandle PhaseTimerHandle;
	FTimerHandle CountdownTimerHandle;
	
	
};
