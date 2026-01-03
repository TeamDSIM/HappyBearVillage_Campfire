// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HBJobBaseComponent.generated.h"


// @PHYTodo: Job 관련 시점 함수
// 현재 호출된 목록
// : OnNightPhaseBegin()
//
// 아직 미호출 된 목록
// : OnGameBegin(), OnDayPhaseBegin(), OnAttack(), OnAction()

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
	// 게임 시작 시점
	virtual void OnGameBegin(); 

	// 낮 시작 시점
	virtual void OnDayPhaseBegin();

	// 밤 시작 시점
	virtual void OnNightPhaseBegin();

	// 공격 시
	virtual void OnAttack();

	// 직업 행동 시
	virtual void OnAction();

protected:
	// 대상 탐지
	virtual AHBCharacterPlayer* DetectTarget();

	// 집 탐지
	virtual AHBHouse* DetectHouse();
	
protected:
	// 직업 스킬 사용 가능 여부
	UPROPERTY()
	uint8 bIsActionActive : 1;

	UPROPERTY()
	TObjectPtr<AHBCharacterPlayer> TargetPlayer = nullptr;

	UPROPERTY()
	TObjectPtr<AHBHouse> TargetHouse = nullptr;

	
};
