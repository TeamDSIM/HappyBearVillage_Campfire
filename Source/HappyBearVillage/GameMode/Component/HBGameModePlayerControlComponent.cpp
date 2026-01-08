// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Component/HBGameModePlayerControlComponent.h"

#include "HappyBearVillage.h"
#include "Algo/RandomShuffle.h"
#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBCharacterColor.h"
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
	// 플레이어 목록 불러오기
	TArray<APlayerState*> Players = InGameState->PlayerArray;

	// 플레이어에게 나눠줄 직업 리스트 초기화
	PlayerJobs.Empty();
	MafiaNum = 0;
	PlayerNum = 0;
	InitPlayersJobList(Players.Num());
	Algo::RandomShuffle(PlayerJobs);

	TArray<FLinearColor> PlayerColors;
	InitPlayersColorList(PlayerColors, Players.Num());
	Algo::RandomShuffle(PlayerColors);
	
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
					// 플레이어의 직업 설정
					PlayerStatComponent->InitCharacterRole(PlayerJobs[i]);

					// 누적 데미지 초기화
					PlayerStatComponent->ResetTotalTakenDamage();
				}

				Character->PlayerColor = PlayerColors[i];
				Character->OnRep_PlayerColor();

				AHBPlayerState* HBPlayerState = Cast<AHBPlayerState>(Players[i]);
				if (HBPlayerState)
				{
					HBPlayerState->SyncPlayerColorFromPlayerStat(Character->PlayerColor);
					HBPlayerState->SetUserID(HBPlayerState->GetPlayerName());
				}

				// 플레이어의 직업 초기화 함수 실행
				UHBJobBaseComponent* JobComponent = Character->GetJobComponent();
				if (JobComponent)
				{
					Character->GetJobComponent()->GameBegin();
				}
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

void UHBGameModePlayerControlComponent::CallJobNightPhaseEnd(AHBMafiaGameState* InGameState)
{
	// Night 시작 시 플레이어 Night 상태 초기화
	for (APlayerState* PS : InGameState->PlayerArray)
	{
		// 플레이어 직업 컴포넌트 저녁 초기화 실행
		AController* PlayerController = PS->GetPlayerController();
		if (PlayerController)
		{
			AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(PlayerController->GetPawn());
			if (Character)
			{
				UHBJobBaseComponent* JobComponent = Character->GetJobComponent();
				if (JobComponent)
				{
					JobComponent->NightPhaseEnd();
				}
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

	// 각 직업 풀을 임시로 생성
	TArray<EJobType> MafiaPool;
	TArray<EJobType> CitizenPool;

	// UEnum 을 사용하여 EJobType의 모든 항목 순회
	const UEnum* JobEnum = StaticEnum<EJobType>();
	if (JobEnum)
	{
		// JobEnum 의 END를 제외하고 순회 돌기
		for (int32 i = 0; i < JobEnum->NumEnums() - 1; ++i)
		{
			EJobType Job = static_cast<EJobType>(JobEnum->GetValueByIndex(i));

			// 구분을 위한 직업군 제외
			if (Job == EJobType::MAFIA || Job == EJobType::CITIZEN || Job == EJobType::END)
			{
				continue;
			}

			if (Job > EJobType::MAFIA && Job < EJobType::CITIZEN)
			{
				MafiaPool.Add(Job);
			}

			else if (Job > EJobType::CITIZEN && Job < EJobType::END)
			{
				CitizenPool.Add(Job);
			}
		}
	}

	// 풀 셔플
	Algo::RandomShuffle(MafiaPool);
	Algo::RandomShuffle(CitizenPool);

	// 디버깅용 직업 설정
	// 마피아와 시민 직업 풀을 이용해서 직업 부여
	// for (int i = 0; i < InPlayerNum; ++i)
	// {
	// 	if (i < MafiaNum)
	// 	{
	// 		PlayerJobs.Add(EJobType::ASSASSIN);
	// 	}
	// 	else
	// 	{
	// 		if (i % 2 == 0)
	// 		{
	// 			PlayerJobs.Add(EJobType::ARMY);
	// 		}
	// 		else
	// 		{
	// 			PlayerJobs.Add(EJobType::POLICE);
	// 		}
	// 	}
	// }

	// 마피아와 시민 직업 풀을 이용해서 직업 부여
	for (int i = 0; i < InPlayerNum; ++i)
	{
		if (i < MafiaNum)
		{
			PlayerJobs.Add(MafiaPool.Pop());
		}
		else
		{
			PlayerJobs.Add(CitizenPool.Pop());
		}
	}
	
	InitPlayerNum(InPlayerNum);
}

void UHBGameModePlayerControlComponent::InitPlayerNum(int InPlayerNum)
{
	PlayerNum = InPlayerNum;
}

void UHBGameModePlayerControlComponent::InitPlayersColorList(TArray<FLinearColor>& OutColors, int InPlayerNum)
{
	TArray<EColorType> ColorPool;

	const UEnum* ColorEnum = StaticEnum<EColorType>();
	if (ColorEnum)
	{
		for (int32 i = 0; i < ColorEnum->NumEnums() - 1; ++i)
		{
			EColorType Color = static_cast<EColorType>(ColorEnum->GetValueByIndex(i));

			if (Color < EColorType::END)
			{
				ColorPool.Add(Color);
			}
		}
	}

	// 풀 셔플
	Algo::RandomShuffle(ColorPool);

	// 마피아와 시민 직업 풀을 이용해서 직업 부여
	for (int i = 0; i < InPlayerNum; ++i)
	{
		FHBCharacterColor Color;
		Color.SetColor(ColorPool.Pop());
		OutColors.Add(Color.GetColorValue());
	}
}


