// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Component/HBGameModePlayerControlComponent.h"

#include "HappyBearVillage.h"
#include "Algo/RandomShuffle.h"
#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameState/HBMafiaGameState.h"
#include "PlayerState/HBPlayerState.h"

// Sets default values for this component's properties
UHBGameModePlayerControlComponent::UHBGameModePlayerControlComponent()
{
	
}

void UHBGameModePlayerControlComponent::InitPlayers(AHBMafiaGameState* InGameState)
{	
	HB_SUBLOG(LogHY, Log, TEXT("InitPlayers"));
	
	// 플레이어 목록 불러오기
	TArray<APlayerState*> Players = InGameState->PlayerArray;

	InitPlayersJobList(Players.Num());
	Algo::RandomShuffle(PlayerJobs);

	// 플레이어 목록을 돌며 직업 배정
	for (int32 i = 0; i < Players.Num(); ++i)
	{
		// 컨트롤러 -> CharacterPlayer -> StatComponent 에 접근
		AController* PlayerController = Players[i]->GetPlayerController();
		if (PlayerController)
		{
			AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(PlayerController->GetPawn());
			if (Character)
			{
				UHBPlayerStatComponent* PlayerStatComponent = Character->GetStat();
				if (PlayerStatComponent)
				{
					UE_LOG(LogTemp, Log, TEXT("Call Server Start Game / InitCharacterRole"));
					// 플레이어의 직업 설정
					PlayerStatComponent->InitCharacterRole(PlayerJobs[i]);

					// 누적 데미지 초기화
					PlayerStatComponent->ResetTotalTakenDamage();

					AHBPlayerState* HBPlayerState = Cast<AHBPlayerState>(Players[i]);
					if (HBPlayerState)
					{
						HBPlayerState->SetUserID(i);
					}
				}

				Character->SetRandomBaseColor();
			}
		}
	}
}

void UHBGameModePlayerControlComponent::ResetPlayers(AHBMafiaGameState* InGameState)
{
	// 플레이어 목록 불러오기
	TArray<APlayerState*> Players = InGameState->PlayerArray;

	// 플레이어 목록을 돌며 스탯 초기화
	for (int32 i = 0; i < Players.Num(); ++i)
	{
		// 컨트롤러 -> CharacterPlayer -> StatComponent 에 접근
		AController* PlayerController = Players[i]->GetPlayerController();
		if (PlayerController)
		{
			AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(PlayerController->GetPawn());
			if (Character)
			{
				UHBPlayerStatComponent* PlayerStatComponent = Character->GetStat();
				if (PlayerStatComponent)
				{
					UE_LOG(LogTemp, Log, TEXT("Call Server Start Game / InitCharacterRole"));
					// 플레이어의 직업 설정
					PlayerStatComponent->ResetCharacterRole();
					
					// 누적 데미지 초기화
					PlayerStatComponent->ResetTotalTakenDamage();
				}

				Character->ResetBaseColor();
			}
		}
	}
	
	UnEquippedAllPlayer(InGameState);
}

void UHBGameModePlayerControlComponent::UnEquippedAllPlayer(AHBMafiaGameState* InGameState)
{
	// 플레이어 목록 불러오기
	TArray<APlayerState*> Players = InGameState->PlayerArray;

	// 플레이어 목록을 돌며 무기 장착 해제
	for (int32 i = 0; i < Players.Num(); ++i)
	{
		// 컨트롤러 -> CharacterPlayer -> StatComponent 에 접근
		AController* PlayerController = Players[i]->GetPlayerController();
		if (PlayerController)
		{
			AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(PlayerController->GetPawn());
			if (Character)
			{
				Character->UnEquipWeapon();
			}
		}
	}
}

void UHBGameModePlayerControlComponent::ResetPlayersTotalTakenDamage(AHBMafiaGameState* InGameState)
{
	// 플레이어 목록 불러오기
	TArray<APlayerState*> Players = InGameState->PlayerArray;

	// 플레이어 목록을 돌며 데미지 초기화
	for (int32 i = 0; i < Players.Num(); ++i)
	{
		// 컨트롤러 -> CharacterPlayer -> StatComponent 에 접근
		AController* PlayerController = Players[i]->GetPlayerController();
		if (PlayerController)
		{
			AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(PlayerController->GetPawn());
			if (Character)
			{
				UHBPlayerStatComponent* PlayerStatComponent = Character->GetStat();
				if (PlayerStatComponent)
				{					
					// 누적 데미지 초기화
					PlayerStatComponent->ResetTotalTakenDamage();
				}
			}
		}
	}
}

void UHBGameModePlayerControlComponent::InitPlayersJobList(int PlayerNum)
{
	// 배치되어야할 마피아 수
	// 8명이 최대일때 5명까지는 1명, 그이상은 2명
	int MafiaNum = PlayerNum - 4 <= 1 ? 1 : PlayerNum / 4 + 1;

	// @PHYTodo : 직업 배치 임시로 진행
	// 일단 현재 마피아와 시민만 배치
	for (int i = 0; i < PlayerNum; ++i)
	{
		if (i < MafiaNum)
		{
			PlayerJobs.Add(EJobType::MAFIA);
		}
		else
		{
			PlayerJobs.Add(EJobType::CITIZEN);
		}
	}
}


