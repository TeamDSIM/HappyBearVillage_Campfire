// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameState/HBMafiaGameState.h"
#include "HBVillageGameMode.generated.h"

class UHBGameModePlayerControlComponent;
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

	// 게임 종료 시 호출
	void StopGame();

	// 치트용 함수
	void CheatPhaseChange();

	// 사람 수 변동 시 호출(승패 확인)
	void CheckGameEnd();

	FORCEINLINE bool GetIsGamePlaying() const { return bIsGamePlaying; }
	FORCEINLINE UHBGameModePlayerControlComponent* GetHBGameModePlayerControlComponent() const
	{
		return GameModePlayerControlComponent;
	}

private:
	virtual void StartPlay() override;
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;

	// PostLogin 호출 시 스팀 세션 접속 인원과
	// 현재 PostLogin 횟수 비교
	void CheckStartGame();

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

	FTimerHandle EndTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState", meta = (AllowPrivateAccess = "true"))
	uint8 bIsGamePlaying : 1;

	int32 ConnectedPlayerCounts = 0;

	uint8 bIsGameEnd : 1;
	uint8 bIsCivilWin : 1;
	uint8 bIsMafiaWin : 1;

	// 컴포넌트 부착
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UHBGameModePlayerControlComponent> GameModePlayerControlComponent;
};
