// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HBCharacterStat.generated.h"

/**
 *	작성일: 2025-12-17
 *  작성자: 박현용
 *  설명: 캐릭터의 기본 스탯을 구조체로 선언
 */

USTRUCT(BlueprintType)
struct FHBCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FHBCharacterStat()
		: AttackDamage(1.f), AttackRange(40.f), AttackRadius(50.f),
		  AttackSpeed(1.f), MovementSpeed(800.f)
	{}

	// 공격 데미지 (투표 시 데미지니까 일반적으로는 1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackDamage;

	// 공격 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	// 공격 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRadius;

	// 공격 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	// 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;
};
