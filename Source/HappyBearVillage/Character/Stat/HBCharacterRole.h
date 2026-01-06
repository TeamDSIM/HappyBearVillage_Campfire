// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HBCharacterRole.generated.h"

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


	END UMETA(DisplayName = "End") // 총 개수
};

UENUM(BlueprintType)
enum class EJobType : uint8
{
	// 마피아 직업군 =======
	MAFIA UMETA(DisplayName = "Mafia"),
	// 
	ASSASSIN UMETA(DisplayName = "Assassin"),	// 완
	FIREBUG UMETA(DisplayName = "FireBug"),
	BOMBER UMETA(DisplayName = "Bomber"),
	
	// 시민 직업군 =========
	CITIZEN UMETA(DisplayName = "Citizen"),
	
	ARMY UMETA(DisplayName = "Army"),	// 완
	SLEEPWALKER UMETA(DisplayName = "Sleepwalker"),
	DETECTOR UMETA(DisplayName = "Detector"),
	SANTA UMETA(DisplayName = "Santa"),
	POLICE UMETA(DisplayName = "Police"),
	INSIDER UMETA(DisplayName = "Insider"),
	STAR UMETA(DisplayName = "Star"),
	POOH UMETA(DisplayName = "Pooh"),
	TREASUREHUNTER UMETA(DisplayName = "TreasureHunter"),

	END UMETA(DisplayName = "End") // 총 개수
};

USTRUCT(BlueprintType)
struct FHBCharacterRole
{
	GENERATED_BODY()

public:
	FHBCharacterRole()
		: Role(ERoleType::CITIZEN), Job(EJobType::CITIZEN)
	{
	}

	// 마피아 / 시민 종류 열거형
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Role)
	ERoleType Role;

	// 상세 직업 열거형
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Role)
	EJobType Job;

	void InitRole()
	{
		FRandomStream RandomStream;
		RandomStream.Initialize(FDateTime::Now().GetTicks());
		
		int32 RoleNum = FMath::RandRange(static_cast<int32>(ERoleType::MAFIA), static_cast<int32>(ERoleType::END) - 1);
		Role = static_cast<ERoleType>(RoleNum);

		int32 JobNum = 0;
		if (Role == ERoleType::CITIZEN)
		{
			JobNum = FMath::RandRange(static_cast<int32>(EJobType::CITIZEN), static_cast<int32>(EJobType::END) - 1);
			Job = static_cast<EJobType>(JobNum);
		}
		else if (Role == ERoleType::MAFIA)
		{
			JobNum = FMath::RandRange(static_cast<int32>(EJobType::MAFIA), static_cast<int32>(EJobType::CITIZEN) - 1);
			Job = static_cast<EJobType>(JobNum);
		}
	}

	void InitRole(EJobType InJob)
	{
		int32 JobNum = static_cast<int32>(InJob);
		Job = InJob;
		
		UE_LOG(LogTemp, Log, TEXT("InitRole Call / JobNum = %d"), JobNum);

		if (JobNum >= static_cast<int32>(EJobType::MAFIA) && JobNum < static_cast<int32>(EJobType::CITIZEN))
		{
			UE_LOG(LogTemp, Log, TEXT("JobNum : %d, Role = MAFIA"), JobNum);
			Role = ERoleType::MAFIA;
		}
		else if(JobNum >= static_cast<int32>(EJobType::CITIZEN) && JobNum < static_cast<int32>(EJobType::END))
		{
			UE_LOG(LogTemp, Log, TEXT("JobNum : %d, Role = CIVIL"), JobNum);
			Role = ERoleType::CITIZEN;
		}
	}
};
