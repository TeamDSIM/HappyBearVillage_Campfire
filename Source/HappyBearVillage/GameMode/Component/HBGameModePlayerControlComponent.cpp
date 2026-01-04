// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Component/HBGameModePlayerControlComponent.h"

#include "HappyBearVillage.h"
#include "Algo/RandomShuffle.h"
#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameState/HBMafiaGameState.h"
#include "PlayerState/HBPlayerState.h"
#include "Subsystem/HBGameVoteSubsystem.h"

// Sets default values for this component's properties
UHBGameModePlayerControlComponent::UHBGameModePlayerControlComponent()
{
	PlayerNum = 0;
}

void UHBGameModePlayerControlComponent::InitPlayers(AHBMafiaGameState* InGameState)
{	
	HB_SUBLOG(LogHY, Log, TEXT("InitPlayers"));
	
	// 플레이어 목록 불러오기
	TArray<APlayerState*> Players = InGameState->PlayerArray;

	// 플레이어에게 나눠줄 직업 리스트 초기화
	PlayerJobs.Empty();
	MafiaNum = 0;
	PlayerNum = 0;
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
					UE_LOG(LogTemp, Log, TEXT("nitCharacterRole, PlayerJobs[%d] : %d"), i, static_cast<int32>(PlayerJobs[i]));
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

				// 플레이어의 직업 초기화 함수 실행
				Character->GetJobComponent()->GameBegin();
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
					AHBPlayerState* HBPlayerState = Cast<AHBPlayerState>(Players[i]);
					if (HBPlayerState)
					{
						HBPlayerState->SyncTotalTakenDamagedFromPlayerStat(PlayerStatComponent->GetTotalTakenDamage());
					}
				}
			}
		}
	}

	UHBGameVoteSubsystem* VoteSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UHBGameVoteSubsystem>();
	if (VoteSubsystem)
	{
		VoteSubsystem->NotifyDamageChanged();
	}
}

void UHBGameModePlayerControlComponent::InitPlayersJobList(int InPlayerNum)
{
	
	// 배치되어야할 마피아 수
	// 8명이 최대일때 5명까지는 1명, 그이상은 2명
	MafiaNum = InPlayerNum - 4 <= 1 ? 1 : InPlayerNum / 4 + 1;
	

	// @PHYTodo : 직업 배치 임시로 진행
	// 일단 현재 마피아와 시민만 배치
	for (int i = 0; i < InPlayerNum; ++i)
	{
		if (i < MafiaNum)
		{
			UE_LOG(LogTemp, Log, TEXT("MAFIA"));
			PlayerJobs.Add(EJobType::MAFIA);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("ARMY"));
			if (i % 2 == 0)
			{
				PlayerJobs.Add(EJobType::ARMY);
			}
			else
			{
				PlayerJobs.Add(EJobType::CITIZEN);
			}
		}
	}
	
	InitPlayerNum(InPlayerNum);
}

void UHBGameModePlayerControlComponent::InitPlayerNum(int InPlayerNum)
{
	PlayerNum = InPlayerNum;
}


