// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/HBVillageGameMode.h"

#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "Component/HBGameModePlayerControlComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "GameState/HBMafiaGameState.h"
#include "PlayerState/HBPlayerState.h"

// HasAuthority 는 액터에서만 사용 가능
// 서버 판별을 위한 구문을 함수로 선언
bool AHBVillageGameMode::IsServer(UWorld* World)
{
	return World && (World->GetNetMode() != NM_Client);
}

AHBVillageGameMode::AHBVillageGameMode()
{
	GameModePlayerControlComponent = CreateDefaultSubobject<UHBGameModePlayerControlComponent>(TEXT("GameModePlayerControl"));
	
	bIsGamePlaying = false;

	// Default 세팅
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(
		TEXT("/Game/Character/Blueprint/BP_HBCharacterPlayer.BP_HBCharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(
		TEXT("/Game/Controller/BP_HBPlayerController.BP_HBPlayerController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	GameStateClass = AHBMafiaGameState::StaticClass();
	PlayerStateClass = AHBPlayerState::StaticClass();

}

void AHBVillageGameMode::StartGame()
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
	AHBMafiaGameState* HBGameState = World->GetGameState<AHBMafiaGameState>();
	if (!HBGameState)
	{
		return;
	}

	// 플레이어 초기 세팅
	GameModePlayerControlComponent->InitPlayers(HBGameState);

	// @PHYTODO : 페이즈 시작
	StartDay();
}

void AHBVillageGameMode::StopGame()
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
	AHBMafiaGameState* HBGameState = World->GetGameState<AHBMafiaGameState>();
	if (!HBGameState)
	{
		return;
	}

	World->GetTimerManager().ClearTimer(PhaseTimerHandle);
	World->GetTimerManager().ClearTimer(CountdownTimerHandle);

	HBGameState->CurrentPhase = EGamePhase::Lobby;
	HBGameState->RemainingTime = 0.f;
	HBGameState->OnRep_GamePhase();
	HBGameState->OnRep_RemainingTime();

	// 플레이어 정보 초기화
	GameModePlayerControlComponent->ResetPlayers(HBGameState);
}

void AHBVillageGameMode::StartPlay()
{
	Super::StartPlay();
	StartGame();
}

// @PHYTODO : 각 페이즈별 함수
// 나중에 각 페이즈별 시간도 데이터로 따로 빼서 변수로 넣어주기
void AHBVillageGameMode::StartDay()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Day"));
	SetPhase(EGamePhase::Day, 10.f);

	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (HBGameState)
	{
		HBGameState->OnRep_GamePhase();
	}

	// 모든 플레이어 장착 해제
	GameModePlayerControlComponent->UnEquippedAllPlayer(HBGameState);
}

void AHBVillageGameMode::StartDiscussion()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Discussion"));
	SetPhase(EGamePhase::Discussion, 30.f);
	
	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (HBGameState)
	{
		HBGameState->OnRep_GamePhase();
	}
}

void AHBVillageGameMode::StartFight()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Fight"));
	SetPhase(EGamePhase::Fight, 20.f);

	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (HBGameState)
	{
		HBGameState->OnRep_GamePhase();
	}

	// 모든 플레이어 데미지 초기화
	GameModePlayerControlComponent->UnEquippedAllPlayer(HBGameState);
}

void AHBVillageGameMode::StartVote()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Vote"));
	SetPhase(EGamePhase::Vote, 25.f);

	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (HBGameState)
	{
		HBGameState->OnRep_GamePhase();
	}
}

void AHBVillageGameMode::StartNight()
{
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] Start Night"));
	SetPhase(EGamePhase::Night, 15.f);

	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (!HBGameState)
	{
		return;
	}

	// 모든 플레이어 장착 해제
	GameModePlayerControlComponent->UnEquippedAllPlayer(HBGameState);

	// Night 시작 시 플레이어 Night 상태 초기화
	for (APlayerState* PS : HBGameState->PlayerArray)
	{
		if (!PS) continue;

		AController* Controller = PS->GetPlayerController();
		if (!Controller) continue;

		AHBCharacterPlayer* Player = Cast<AHBCharacterPlayer>(Controller->GetPawn());
		if (!Player) continue;

		Player->ResetNightState();
	}

	HBGameState->OnRep_GamePhase();
}


void AHBVillageGameMode::SetPhase(EGamePhase NewPhase, float Duration)
{
	// 서버에서 처리하도록 예외처리
	UWorld* World = GetWorld();
	if (!IsServer(World))
	{
		return;
	}

	// 페이즈 관리를 위해 HBMafiaGameState 불러오기
	AHBMafiaGameState* HBGameState = World->GetGameState<AHBMafiaGameState>();
	if (!HBGameState)
	{
		return;
	}

	// GameState 의 현재 페이즈와 남은 시간을 페이즈에 맞게 설정
	UE_LOG(LogTemp, Log, TEXT("[GameFlowSubsystem] SetPhase / Duration"));
	HBGameState->CurrentPhase = NewPhase;
	HBGameState->RemainingTime = Duration;

	World->GetTimerManager().ClearTimer(CountdownTimerHandle);
	StartCountdown(Duration);
	HBGameState->OnRep_RemainingTime();

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

void AHBVillageGameMode::StartCountdown(float StartTime)
{
	UWorld* World = GetWorld();
	// 서버에서만 시간 관리 로직 실행
	if (!IsServer(World))
	{
		return;
	}

	AHBMafiaGameState* HBGameState = World->GetGameState<AHBMafiaGameState>();
	if (!HBGameState)
	{
		return;
	}

	HBGameState->RemainingTime = StartTime;

	World->GetTimerManager().SetTimer(
		CountdownTimerHandle,
		this,
		&AHBVillageGameMode::TickCountdown,
		1.f,
		true
	);
}

void AHBVillageGameMode::TickCountdown()
{
	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (HBGameState)
	{
		HBGameState->RemainingTime = FMath::Max(0.f, HBGameState->RemainingTime - 1.f);

		FString PhaseName
			= StaticEnum<EGamePhase>()->GetNameStringByValue(static_cast<int32>(HBGameState->CurrentPhase));

		UE_LOG(LogTemp, Log, TEXT("[Phase : %s], Remaining Time : %f"),
		       *PhaseName, HBGameState->RemainingTime);

		if (HBGameState->RemainingTime <= 0.f)
		{
			GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		}

		HBGameState->OnRep_RemainingTime();
	}
}