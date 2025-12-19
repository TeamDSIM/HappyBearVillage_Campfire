// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/HBGameFlowSubsystem.h"

#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

// HasAuthority 는 액터에서만 사용 가능
// 서버 판별을 위한 구문을 함수로 선언
bool IsServer(UWorld* World)
{
	return World && (World->GetNetMode() != NM_Client);
}

void UHBGameFlowSubsystem::StartGame()
{
	UE_LOG(LogTemp, Log, TEXT("StartGame Call"));
	UWorld* World = GetWorld();

	// 서버가 아니라면 반환
	if (!IsServer(World))
	{
		return;
	}

	// 서버라면 직업배정
	AGameStateBase* GS = World->GetGameState();
	if (!GS)
	{
		 return;
	}

	// 플레이어 목록 불러오기
	TArray<APlayerState*> Players = GS->PlayerArray;

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
					PlayerStatComponent->InitCharacterRole();
				}

				Character->SetRandomBaseColor();
			}
		}
	}
	

	// @PHYTODO : 페이즈 시작
}
