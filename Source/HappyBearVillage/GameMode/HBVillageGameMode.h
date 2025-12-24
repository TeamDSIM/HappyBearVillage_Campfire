// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameState/HBMafiaGameState.h"
#include "HBVillageGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API AHBVillageGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHBVillageGameMode();

	// 게임 시작 시 호출될 함수
	void StartGame();
	void StopGame();

	FORCEINLINE bool GetIsGamePlaying() const { return bIsGamePlaying; }

private:
	virtual void StartPlay() override;
	
	// 각 페이즈 별 시작 함수
	void StartDay(); // 낮 (토론 전 잠시 대기)
	void StartDiscussion(); // 토론
	void StartFight(); // 난투
	void StartVote(); // 투표
	void StartNight(); // 밤

	// 페이즈 변환 함수
	void SetPhase(EGamePhase NewPhase, float Duration);

	// 시간초 감소 함수
	void StartCountdown(float StartTime);
	void TickCountdown();

	bool IsServer(UWorld* World);

	// 각 페이즈 별 시간 관리할 타이머
	FTimerHandle PhaseTimerHandle;
	FTimerHandle CountdownTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState", meta = (AllowPrivateAccess = "true"))
	uint8 bIsGamePlaying : 1;

	// 컴포넌트 부착
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UHBGameModePlayerControlComponent> GameModePlayerControlComponent;
	
	
};
