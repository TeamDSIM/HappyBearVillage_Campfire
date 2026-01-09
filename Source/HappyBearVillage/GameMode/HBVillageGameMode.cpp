// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/HBVillageGameMode.h"

#include "HappyBearVillage.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "Component/HBCharacterRelocationComponent.h"
#include "Component/HBGameModePlayerControlComponent.h"
#include "Component/HBGameModeVillageGenerationComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "GameState/HBMafiaGameState.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PlayerState/HBPlayerState.h"
#include "Subsystem/HBGameVoteSubsystem.h"

AHBVillageGameMode::AHBVillageGameMode()
{
	GameModePlayerControlComponent = CreateDefaultSubobject<UHBGameModePlayerControlComponent>(
		TEXT("GameModePlayerControl"));
	GameModeVillageGenerationComponent = CreateDefaultSubobject<UHBGameModeVillageGenerationComponent>(
		TEXT("GameModeVillageGeneration"));
	CharacterRelocationComponent = CreateDefaultSubobject<UHBCharacterRelocationComponent>(
		TEXT("CharacterRelocationComponent"));

	bIsGamePlaying = false;

	// Default 세팅
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(
		TEXT("/Game/Character/Blueprint/BP_HBCharacterPlayer.BP_HBCharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(
		TEXT("/Game/Controller/BP_HBVillagePlayerController.BP_HBVillagePlayerController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	GameStateClass = AHBMafiaGameState::StaticClass();
	PlayerStateClass = AHBPlayerState::StaticClass();

	ReadyPlayerCount = 0;

	bIsGameEnd = false;
	bIsCivilWin = false;
	bIsMafiaWin = false;

	bUseSeamlessTravel = true;
}

void AHBVillageGameMode::ApplyClientReady()
{
	++ReadyPlayerCount;

	HB_LOG(LogTemp, Log, TEXT("ApplyClientReady Call : ReadyPlayerCount : %d"), ReadyPlayerCount);

	IOnlineSessionPtr SessionInterface = Online::GetSubsystem(GetWorld())->GetSessionInterface();
	if (!SessionInterface.IsValid()) return;

	FNamedOnlineSession* Session = SessionInterface->GetNamedSession(NAME_GameSession);
	if (!Session) return;
	
	const int32 MaxPlayerCount = Session->SessionSettings.NumPublicConnections;
	const int32 ExpectedPlayerCount = MaxPlayerCount - Session->NumOpenPublicConnections;

	HB_LOG(LogTemp, Log, TEXT("ApplyClientReady State : ReadyPlayerCount : %d, ExpectedPlayerCount : %d"), ReadyPlayerCount, ExpectedPlayerCount);

	if (ReadyPlayerCount == ExpectedPlayerCount) StartGame();
}

void AHBVillageGameMode::StartGame()
{
	HB_LOG(LogTemp, Log, TEXT("StartGame Call"));
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

	HBGameState->GameProgress = EGameProgress::Playing;
	HBGameState->OnRep_GameProgress();

	// 플레이어 초기 세팅
	GameModePlayerControlComponent->InitPlayers(HBGameState);
	GameModeVillageGenerationComponent->SyncVillageGenerationData(HBGameState);

	// 날짜 세팅
	HBGameState->Date = 0;

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

	bIsGameEnd = false;
	bIsCivilWin = false;
	bIsMafiaWin = false;

	HBGameState->CurrentPhase = EGamePhase::Lobby;
	HBGameState->RemainingTime = 0.f;
	HBGameState->Date = 0;
	HBGameState->OnRep_GamePhase();
	HBGameState->OnRep_RemainingTime();
	HBGameState->OnRep_Date();

	// 플레이어 정보 초기화
	GameModePlayerControlComponent->ResetPlayers(HBGameState);
	
	bUseSeamlessTravel = true;

	//@ Todo : 맵 이름 변경
	FString Map = TEXT("/Game/Maps/LobbyMap");
	World->ServerTravel(Map);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC) continue;

		APawn* Pawn = PC->GetPawn();
		if (!Pawn) continue;

		FVector LobbyRespawnLocation;
		LobbyRespawnLocation.X = 0.0f;
		LobbyRespawnLocation.Y = 0.0f;
		LobbyRespawnLocation.Z = 92.0f;

		Pawn->SetActorLocation(LobbyRespawnLocation);
	}

}

void AHBVillageGameMode::CheckGameEnd()
{
	UE_LOG(LogTemp, Log, TEXT("CheckGameEnd Call"));
	// 남은 마피아 수가 0이면
	if (GetHBGameModePlayerControlComponent()->GetMafiaNum() == 0)
	{
		// 플레이어 승리
		bIsCivilWin = true;

		// 종료 플래그 설정
		bIsGameEnd = true;
	}

	// 남은 시민 수가 마피아랑 같거나 적으면
	else if (GetHBGameModePlayerControlComponent()->GetPlayerNum() - GetHBGameModePlayerControlComponent()->
		GetMafiaNum()
		<= GetHBGameModePlayerControlComponent()->GetMafiaNum())
	{
		// 마피아 승리
		bIsMafiaWin = true;

		// 종료 플래그 설정
		bIsGameEnd = true;
	}

	// 종료 플래그가 참일 시 모든 타이머를 멈추고 종료 페이즈로 전환
	if (bIsGameEnd)
	{
		UE_LOG(LogTemp, Log, TEXT("CheckGameEnd Timer Call"));
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(PhaseTimerHandle);

		// 승패 화면 출력
		// GameState 의 GameEnd 를 변경하여 RepNotify 호출
		AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
		if (HBGameState)
		{
			if (bIsMafiaWin)
			{
				HBGameState->GameEnd = 1;
			}
			else
			{
				HBGameState->GameEnd = 2;
			}

			HBGameState->OnRep_GameEnd();
		}

		// 타이머로 일정 시간 뒤 StopGame();
		GetWorld()->GetTimerManager().SetTimer(
			EndTimerHandle,
			FTimerDelegate::CreateLambda(
				[this]()
				{
					StopGame();
				}
			), 5.f, false
		);
	}
}

void AHBVillageGameMode::CheatPhaseChange()
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
	SetPhase(HBGameState->CurrentPhase, 1);
}

// @PHYTODO : 각 페이즈별 함수
// 나중에 각 페이즈별 시간도 데이터로 따로 빼서 변수로 넣어주기
void AHBVillageGameMode::StartDay()
{
	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (!HBGameState)
	{
		return;
	}
	
	SetPhase(EGamePhase::Day, 3.f);
	HBGameState->OnRep_GamePhase();
	
	HBGameState->Date += 1;
	HBGameState->OnRep_Date();
	
	CharacterRelocationComponent->RelocateCharacterToCouncil(HBGameState);
	
	// Day 시작 시 플레이어 Day 상태 초기화
	for (APlayerState* PS : HBGameState->PlayerArray)
	{
		// 플레이어 직업 컴포넌트 낮 초기화 실행
		AController* PlayerController = PS->GetPlayerController();
		if (PlayerController)
		{
			AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(PlayerController->GetPawn());
			if (Character)
			{
				Character->RenderColor = Character->PlayerColor;
				Character->OnRep_RenderColor();
				
				UHBJobBaseComponent* JobComponent = Character->GetJobComponent();
				if (JobComponent)
				{
					JobComponent->DayPhaseBegin();
				}
			}
		}
	}

	// 모든 플레이어 장착 해제
	GameModePlayerControlComponent->UnEquippedAllPlayer(HBGameState);
}

void AHBVillageGameMode::StartDiscussion()
{
	SetPhase(EGamePhase::Discussion, 30.f);

	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (HBGameState)
	{
		HBGameState->OnRep_GamePhase();
	}
	else
	{
		return;
	}
}

void AHBVillageGameMode::StartFight()
{
	//SetPhase(EGamePhase::Fight, 20.f);
		//Test 위해 시간 늘렸습니다.
	SetPhase(EGamePhase::Fight, 50.f);

	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (HBGameState)
	{
		HBGameState->OnRep_GamePhase();
	}
	else
	{
		return;
	}

	// 모든 플레이어 데미지 초기화
	HB_LOG(LogHY, Log, TEXT("Start Fight Reset Damage"));
	GameModePlayerControlComponent->ResetPlayersTotalTakenDamage(HBGameState);
	HBGameState->OnRep_TopDamagePlayers();
	//GameModePlayerControlComponent->UnEquippedAllPlayer(HBGameState);
}

void AHBVillageGameMode::StartVote()
{
	//SetPhase(EGamePhase::Vote, 25.f);
	//Test 위해 시간 늘렸습니다.
	SetPhase(EGamePhase::Vote, 50.f);

	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (HBGameState)
	{
		HBGameState->OnRep_GamePhase();

		// 투표 대상 불러와서 설정
		if (HBGameState->TopDamagePlayers[0].TotalTakenDamaged == 0)
		{
		}
		// 데미지가 0이라면
		else
		{
			// 컨트롤러 -> CharacterPlayer -> StatComponent 에 접근
			AController* PlayerController = HBGameState->TopDamagePlayers[0].PlayerState->GetPlayerController();
			if (PlayerController)
			{
				AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(PlayerController->GetPawn());
				if (Character)
				{
					UHBPlayerStatComponent* PlayerStatComponent = Character->GetStat();
					if (PlayerStatComponent)
					{
						PlayerStatComponent->SetIsVoteTarget(true);

						UHBGameVoteSubsystem* VoteSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<
							UHBGameVoteSubsystem>();
						if (VoteSubsystem)
						{
							VoteSubsystem->SetCurrentVoteTarget(Character);
							CharacterRelocationComponent->RelocateCharacterToExecutePlatform(HBGameState);
							HBGameState->TargetVoteNum = 0;
							HBGameState->OnRep_TargetVoteNum();
						}
					}
				}
			}
		}
	}
	else
	{
		return;
	}
}

void AHBVillageGameMode::StartVoteCheck()
{
	SetPhase(EGamePhase::VoteCheck, 3.f);
	
	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (HBGameState)
	{
		HBGameState->OnRep_GamePhase();
	}
}

void AHBVillageGameMode::StartNight()
{

	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (!HBGameState)
	{
		return;
	}
	SetPhase(EGamePhase::Night, 180.f);
	HBGameState->OnRep_GamePhase();
	
	//모든 플레이어 집에 스폰
	CharacterRelocationComponent->RelocateCharactersToHouse(HBGameState);

	// 모든 플레이어 장착 해제
	GameModePlayerControlComponent->UnEquippedAllPlayer(HBGameState);

	// 투표 대상 초기화
	UHBGameVoteSubsystem* VoteSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UHBGameVoteSubsystem>();
	if (VoteSubsystem)
	{
		VoteSubsystem->ClearCurrentVoteTarget();
	}
	
	// Night 시작 시 플레이어 Night 상태 초기화
	for (APlayerState* PS : HBGameState->PlayerArray)
	{
		// 플레이어 직업 컴포넌트 저녁 초기화 실행 (구조상 가장 마지막에 실행해야 함)
		AController* PlayerController = PS->GetPlayerController();
		if (PlayerController)
		{
			AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(PlayerController->GetPawn());
			if (Character)
			{
				Character->RenderColor = FLinearColor::Black;
				Character->OnRep_RenderColor();
				
				UHBJobBaseComponent* JobComponent = Character->GetJobComponent();
				if (JobComponent)
				{
					JobComponent->NightPhaseBegin();
				}
			}
		}
		
		if (!PS) continue;

		AController* Controller = PS->GetPlayerController();
		if (!Controller) continue;

		AHBCharacterPlayer* Player = Cast<AHBCharacterPlayer>(Controller->GetPawn());
		if (!Player) continue;

		Player->ResetNightState();
	}

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
	HBGameState->CurrentPhase = NewPhase;
	HBGameState->RemainingTime = Duration;

	World->GetTimerManager().ClearTimer(CountdownTimerHandle);
	World->GetTimerManager().ClearTimer(PhaseTimerHandle);
	if (!bIsGameEnd)
	{
		StartCountdown(Duration);
	}
	HBGameState->OnRep_RemainingTime();

	// 각 페이즈 종료 시 다음 페이즈로 넘겨줄 델리게이트 생성
	World->GetTimerManager().SetTimer(
		PhaseTimerHandle,
		FTimerDelegate::CreateLambda(
			[this, NewPhase]()
			{
				AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
				if (!HBGameState)
				{
					return;
				}
				
				switch (NewPhase)
				{
				case EGamePhase::Day:
					StartDiscussion();
					break;

				case EGamePhase::Discussion:
					{
						// 첫날이면 Night 로 스킵
						if (HBGameState->Date == 1)
						{
							StartVoteCheck();
						}
						// 첫날이 아니면 fight 로 이동
						else
						{
							StartFight();
						}
					}
					break;

				case EGamePhase::Fight:
					StartVote();
					break;

				case EGamePhase::Vote:
					{
						// 밤으로 전환 전에 사망 체크
						UHBGameVoteSubsystem* VoteSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<
							UHBGameVoteSubsystem>();
						if (VoteSubsystem)
						{
							VoteSubsystem->CheckTargetIsDead();
						}

						if (!bIsGameEnd)
						{
							StartVoteCheck();
						}
					}
					break;
					
				case EGamePhase::VoteCheck:
					StartNight();
					break;

				case EGamePhase::Night:
					{
						// 직업의 밤 종료 함수 호출
						GetHBGameModePlayerControlComponent()->CallJobNightPhaseEnd(HBGameState);
						StartDay();
					}
					break;

				default:
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

		if (HBGameState->RemainingTime <= 0.f)
		{
			GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		}

		HBGameState->OnRep_RemainingTime();
	}
}

// HasAuthority 는 액터에서만 사용 가능
// 서버 판별을 위한 구문을 함수로 선언
bool AHBVillageGameMode::IsServer(UWorld* World)
{
	return World && (World->GetNetMode() != NM_Client);
}