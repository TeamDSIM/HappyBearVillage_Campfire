// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *	작성일: 2025-12-18
 *  작성자: 박현용
 *  설명: 캐릭터의 직업 관련 열거형 및 구조체 선언
 */

UENUM(BlueprintType)
enum class ERoleType : uint8
{
	MAFIA UMETA(DisplayName = "Mafia"),
	CITIZEN UMETA(DisplayName = "Citizen"),


	END UMETA(DisplayName = "End")	// 총 개수를 나타내기 위한 마지막 선언
};

UENUM(BlueprintType)
enum class EMafiaRole : uint8
{
	//
};

USTRUCT(BlueprintType)
struct FHBCharacterRole : public FTableRowBase
{
	GENERATED_BODY()

public:
	FHBCharacterRole()
	{}

	// 마피아 / 시민 종류 열거형

	// 상세 직업 열거형
	
};