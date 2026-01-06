// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameState/HBMafiaGameState.h"
#include "HBJobBaseComponent.generated.h"


// @PHYTodo: Job 관련 시점 함수
// 현재 호출된 목록
// : OnNightPhaseBegin(), OnGameBegin(), OnDayPhaseBegin(), OnAction()
//
// 아직 미호출 된 목록
// : OnAttack(), 

class AHBHouse;
class AHBCharacterPlayer;

UCLASS(Abstract ,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAPPYBEARVILLAGE_API UHBJobBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHBJobBaseComponent();

public:
	// 직업 호출 시점
	// 게임 시작 시점 (GameMode -> StartGame -> InitPlayer 마지막)
	virtual void GameBegin();

	// 낮 시작 시점 (GameMode -> StartDay)
	virtual void DayPhaseBegin();

	// 밤 시작 시점 (GameMode -> StartNight)
	virtual void NightPhaseBegin();

	// 공격 시 (CharacterPlayer -> AttackHitConfirm 에서 호출, 피격 대상을 인자로 받음)
	virtual void Attack(AActor* HitActor);

	// 직업 행동 시 (PlayerCharcter -> DoJobAction()
	virtual void Action();

protected:
	// 대상 탐지
	virtual AHBCharacterPlayer* DetectTarget();
	virtual TArray<AHBCharacterPlayer*> DetectTargets();

	// 집 탐지
	virtual AHBHouse* DetectHouse();

	// 현재 Phase 반환
	virtual EGamePhase GetCurrentPhase();
	
protected:
	// 직업 스킬 사용 가능 여부
	UPROPERTY()
	uint8 bIsActionActive : 1;	
};
