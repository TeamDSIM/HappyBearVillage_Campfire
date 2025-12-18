// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HBGameFlowSubsystem.generated.h"

/**
*	작성일: 2025-12-18
 *  작성자: 박현용
 *  설명: 마피아 게임 플로우를 관리하는 서브시스템
 */

// 각 페이즈를 나타내는 enum 클래스
UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	Waiting,	// 대기 상태
	Discussion,	// 토론 상태
	Fight,		// 난투 상태
	Vote,		// 최종 투표 상태
	Night,		// 밤 상태

	End,
};

UCLASS()
class HAPPYBEARVILLAGE_API UHBGameFlowSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 게임 시작 시 호출될 함수
	void StartGame();

private:
	// @PHYTODO : 각 페이즈 별 시작 함수

	// @PHYTODO : 페이즈 변환 함수
	
	
	// 각 페이즈 별 시간 관리할 타이머
	FTimerHandle PhaseTimerHandle;
	
	
};
