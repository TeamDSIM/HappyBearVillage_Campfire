// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/HBGameFlowSubsystem.h"

#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "GameState/HBMafiaGameState.h"

// HasAuthority 는 액터에서만 사용 가능
// 서버 판별을 위한 구문을 함수로 선언
bool UHBGameFlowSubsystem::IsServer(UWorld* World)
{
	return World && (World->GetNetMode() != NM_Client);
}

UHBGameFlowSubsystem::UHBGameFlowSubsystem()
{
	bIsGamePlaying = false;
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

	// 게임이 실행중이라면 반환
	if (bIsGamePlaying)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game is Playing"));
		return;
	}

	bIsGamePlaying = true;

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

					// 누적 데미지 초기화
					PlayerStatComponent->ResetTotalTakenDamage();
				}

				Character->SetRandomBaseColor();
			}
		}
	}


	// @PHYTODO : 페이즈 시작
	StartDay();
}

void UHBGameFlowSubsystem::StopGame()
{
	// 서버에서 처리하도록 예외처리
	UWorld* World = GetWorld();
	if (!IsServer(World))
	{
		return;
	}

	// 게임이 진행중이 아니면
	if (!bIsGamePlaying)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game isn't Playing"));
		return;
	}

	bIsGamePlaying = false;
	
	// 페이즈 관리를 위해 HBMafiaGameState 불러오기
	AHBMafiaGameState* GameState = World->GetGameState<AHBMafiaGameState>();
	if (!GameState)
	{
		return;
	}

	World->GetTimerManager().ClearTimer(PhaseTimerHandle);
	World->GetTimerManager().ClearTimer(CountdownTimerHandle);

	GameState->CurrentPhase = EGamePhase::Lobby;
	GameState->RemainingTime = 0.f;
	GameState->OnRep_GamePhase();
	GameState->OnRep_RemainingTime();

	// 플레이어 목록 불러오기
	TArray<APlayerState*> Players = GameState->PlayerArray;

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
	
	UnEquippedAllPlayer(GameState);
}

// @PHYTODO : 각 페이즈별 함수
// 나중에 각 페이즈별 시간도 데이터로 따로 빼서 변수로 넣어주기
void UHBGameFlowSubsystem::StartDay()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Day"));
	SetPhase(EGamePhase::Day, 10.f);

	AHBMafiaGameState* GameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (GameState)
	{
		GameState->OnRep_GamePhase();
	}
	
	UnEquippedAllPlayer(GameState);
}

void UHBGameFlowSubsystem::StartDiscussion()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Discussion"));
	SetPhase(EGamePhase::Discussion, 30.f);
	
	AHBMafiaGameState* GameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (GameState)
	{
		GameState->OnRep_GamePhase();
	}
}

void UHBGameFlowSubsystem::StartFight()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Fight"));
	SetPhase(EGamePhase::Fight, 20.f);

	AHBMafiaGameState* GameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (GameState)
	{
		GameState->OnRep_GamePhase();
	}

	// 플레이어 목록 불러오기
	TArray<APlayerState*> Players = GameState->PlayerArray;

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

void UHBGameFlowSubsystem::StartVote()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Vote"));
	SetPhase(EGamePhase::Vote, 25.f);

	AHBMafiaGameState* GameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (GameState)
	{
		GameState->OnRep_GamePhase();
	}
}

void UHBGameFlowSubsystem::StartNight()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Night"));
	SetPhase(EGamePhase::Night, 15.f);

	AHBMafiaGameState* GameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (GameState)
	{
		GameState->OnRep_GamePhase();
	}

	UnEquippedAllPlayer(GameState);
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
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] SetPhase / Duration"));
	GameState->CurrentPhase = NewPhase;
	GameState->RemainingTime = Duration;

	World->GetTimerManager().ClearTimer(CountdownTimerHandle);
	StartCountdown(Duration);
	GameState->OnRep_RemainingTime();

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

		FString PhaseName
			= StaticEnum<EGamePhase>()->GetNameStringByValue(static_cast<int32>(GameState->CurrentPhase));

		UE_LOG(LogTemp, Log, TEXT("[Phase : %s], Remaining Time : %f"),
		       *PhaseName, GameState->RemainingTime);

		if (GameState->RemainingTime <= 0.f)
		{
			GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		}

		GameState->OnRep_RemainingTime();
	}
}

void UHBGameFlowSubsystem::UnEquippedAllPlayer(AHBMafiaGameState* InGameState)
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
