// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HBAttackAnimationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHBAttackAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	작성일: 2025-12-16
 *  작성자: 박현용
 *  설명: 공격 애니메이션에 들어가는 노티파이에서 캐릭터로 피격 판정 체크를 전달해주는 인터페이스
 */

class HAPPYBEARVILLAGE_API IHBAttackAnimationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AttackHitCheck() = 0;
};
