// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/HBGameVoteSubsystem.h"

#include "GameState/HBMafiaGameState.h"

// HasAuthority 는 액터에서만 사용 가능
// 서버 판별을 위한 구문을 함수로 선언
bool UHBGameVoteSubsystem::IsServer(UWorld* World)
{
	return World && (World->GetNetMode() != NM_Client);
}

UHBGameVoteSubsystem::UHBGameVoteSubsystem()
{
	bRankingDirty = false;
	bUpdateSchduled = false;
}

void UHBGameVoteSubsystem::CalculateTop3DamagePlayers()
{
	UE_LOG(LogTemp, Log, TEXT("[UHBGameVoteSubsystem] Call CalculateTop3DamagePlayers"));
	UWorld* World = GetWorld();
	if (World == nullptr || !IsServer(World))
	{
		return;
	}

	AHBMafiaGameState* GS = World->GetGameState<AHBMafiaGameState>();
	if (!GS)
	{
		return;
	}

	TArray<FDamageRankEntry> RankList;

	// 플레이어 목록 불러오기
	TArray<APlayerState*> Players = GS->PlayerArray;

	// 플레이어 목록을 돌며 직업 배정
	for (int32 i = 0; i < Players.Num(); ++i)
	{
		AHBPlayerState* HBPlayerState = Cast<AHBPlayerState>(Players[i]);
		if (HBPlayerState)
		{
			RankList.Add({HBPlayerState, HBPlayerState->GetTotalTakenDamaged()});
		}
	}

	// 누적 전체 데미지 순으로 정렬
	RankList.Sort(
		[](const FDamageRankEntry& A, FDamageRankEntry B)
		{
			return A.TotalTakenDamaged > B.TotalTakenDamaged;
		});

	if (RankList.Num() > 3)
	{
		RankList.SetNum(3);
	}

	GS->SetTopDamagePlayers(RankList);
}

void UHBGameVoteSubsystem::NotifyDamageChanged()
{
	UE_LOG(LogTemp, Log, TEXT("[UHBGameVoteSubsystem] Call NotifyDamageChanged"));
	
	// 서버에서 처리하도록 예외처리
	UWorld* World = GetWorld();
	if (!IsServer(World))
	{
		UE_LOG(LogTemp, Error, TEXT("[UHBGameVoteSubsystem] No World"));
		return;
	}

	// 랭킹 정렬 필요 플래그 설정
	bRankingDirty = true;

	// 이미 업뎃 예약 시 중복 예약 방지
	if (bUpdateSchduled)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UHBGameVoteSubsystem] UpdateShceduled is True"));
		return;
	}

	bUpdateSchduled = true;

	// 한 틱 뒤에 랭킹 업데이트
	GetWorld()->GetTimerManager().SetTimerForNextTick(
		[this]()
		{
			UpdateRanking();
		});
}

void UHBGameVoteSubsystem::UpdateRanking()
{
	UE_LOG(LogTemp, Log, TEXT("[UHBGameVoteSubsystem] Call UpdateRanking"));
	if (!bRankingDirty)
	{
		return;
	}

	bRankingDirty = false;
	bUpdateSchduled = false;

	CalculateTop3DamagePlayers();
}
