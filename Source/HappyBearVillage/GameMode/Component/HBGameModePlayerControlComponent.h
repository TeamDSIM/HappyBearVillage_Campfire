// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Stat/HBCharacterRole.h"
#include "Components/ActorComponent.h"
#include "HBGameModePlayerControlComponent.generated.h"


class AHBMafiaGameState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAPPYBEARVILLAGE_API UHBGameModePlayerControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHBGameModePlayerControlComponent();

public:
	FORCEINLINE int32 GetPlayerNum() const { return PlayerNum; }
	FORCEINLINE int32 GetMafiaNum() const { return MafiaNum; }
	
	FORCEINLINE void SetPlayerNum(int32 InPlayerNum) { PlayerNum = InPlayerNum; }
	FORCEINLINE void SetMafiaNum(int32 InMafiaNum) { MafiaNum = InMafiaNum; }
	
	
public:
	// 플레이어 전체 정보 초기설정 / 리셋
	void InitPlayers(AHBMafiaGameState* InGameState);
	void ResetPlayers(AHBMafiaGameState* InGameState);

	// 플레이어 전체 무기 장착 해제
	void UnEquippedAllPlayer(AHBMafiaGameState* InGameState);

	// 플레이어 전체 누적 데미지 초기화
	void ResetPlayersTotalTakenDamage(AHBMafiaGameState* InGameState);

protected:
	

private:
private:
	void InitPlayersJobList(int InPlayerNum);
	void InitPlayerNum(int InPlayerNum);
	
	TArray<EJobType> PlayerJobs;
	// 생존중인 플레이어 숫자
	int32 PlayerNum = 0;

	// 생존중인 마피아 숫자
	int32 MafiaNum = 0;
};
