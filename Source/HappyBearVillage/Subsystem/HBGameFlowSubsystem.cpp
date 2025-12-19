// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/HBGameFlowSubsystem.h"

#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "GameState/HBMafiaGameState.h"

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

// @PHYTODO : 각 페이즈별 함수
// 나중에 각 페이즈별 시간도 데이터로 따로 빼서 변수로 넣어주기
void UHBGameFlowSubsystem::StartDay()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Day"));
	SetPhase(EGamePhase::Day, 10.f);
}

void UHBGameFlowSubsystem::StartDiscussion()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Discussion"));
	SetPhase(EGamePhase::Discussion, 30.f);
}

void UHBGameFlowSubsystem::StartFight()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Fight"));
	SetPhase(EGamePhase::Fight, 20.f);
}

void UHBGameFlowSubsystem::StartVote()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Vote"));
	SetPhase(EGamePhase::Vote, 25.f);
}

void UHBGameFlowSubsystem::StartNight()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Night"));
	SetPhase(EGamePhase::Night, 15.f);
}

void UHBGameFlowSubsystem::SetPhase(EGamePhase NewPhase, float Duration)
{
	// 서버에서 처리하도록 예외처리
	UWorld* World = GetWorld();
	if (!IsServer(World))
	{
		return;
	}

	// 페이즈 관리를 위해 HBMafiaGameState 불러오기
	AHBMafiaGameState* GameState = World->GetGameState<AHBMafiaGameState>();
	if (!GameState)
	{
		return;
	}

	// GameState 의 현재 페이즈와 남은 시간을 페이즈에 맞게 설정
	GameState->CurrentPhase = NewPhase;
	GameState->RemainingTime = Duration;

	// 각 페이즈 종료 시 다음 페이즈로 넘겨줄 델리게이트 생성
	World->GetTimerManager().SetTimer(
		PhaseTimerHandle,
		FTimerDelegate::CreateLambda(
			[this, NewPhase]()
			{
				switch (NewPhase)
				{
				case EGamePhase::Day:
					StartDiscussion();
					break;

				case EGamePhase::Discussion:
					StartFight();
					break;

				case EGamePhase::Fight:
					StartVote();
					break;

				case EGamePhase::Vote:
					StartNight();
					break;

				case EGamePhase::Night:
					StartDay();
					break;

				default:
					UE_LOG(LogTemp, Error, TEXT("this GamePhase is Not Valid"));
					break;
				}
			}),
		Duration, false
	);
}

void UHBGameFlowSubsystem::StartCountdown(float StartTime)
{
	UWorld* World = GetWorld();
	// 서버에서만 시간 관리 로직 실행
	if (!IsServer(World))
	{
		return;
	}

	AHBMafiaGameState* GameState = World->GetGameState<AHBMafiaGameState>();
	if (!GameState)
	{
		return;
	}

	GameState->RemainingTime = StartTime;

	World->GetTimerManager().SetTimer(
		CountdownTimerHandle,
		this,
		&UHBGameFlowSubsystem::TickCountdown,
		1.f,
		true
	);
}

void UHBGameFlowSubsystem::TickCountdown()
{
	AHBMafiaGameState* GameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (GameState)
	{
		GameState->RemainingTime = FMath::Max(0.f, GameState->RemainingTime - 1.f);

		if (GameState->RemainingTime <= 0.f)
		{
			GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		}
	}
}
