// Fill out your copyright notice in the Description page of Project Settings.


#include "HBVillagePlayerController.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HappyBearVillage.h"
#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "Component/HBInGameHUDComponent.h"
#include "Component/HBJobInfoWidgetComponent.h"
#include "Component/HBMapWidgetComponent.h"
#include "Component/HBMinimapWidgetComponent.h"
#include "GameMode/HBVillageGameMode.h"

AHBVillagePlayerController::AHBVillagePlayerController()
{
	InGameHUDComponent = CreateDefaultSubobject<UHBInGameHUDComponent>(TEXT("InGameHUD"));
	MinimapWidgetComponent = CreateDefaultSubobject<UHBMinimapWidgetComponent>(TEXT("Minimap"));
	MapWidgetComponent = CreateDefaultSubobject<UHBMapWidgetComponent>(TEXT("Map"));
	JobInfoWidgetComponent = CreateDefaultSubobject<UHBJobInfoWidgetComponent>(TEXT("JobInfo"));

	static ConstructorHelpers::FObjectFinder<UInputAction> ToggleMapActionRef(TEXT("/Game/Character/Input/Action/IA_ToggleMap.IA_ToggleMap"));
	if (ToggleMapActionRef.Succeeded())
	{
		ToggleMapAction = ToggleMapActionRef.Object;
	}
}

void AHBVillagePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) return;
	
	HB_LOG(LogTemp, Log, TEXT("PlayerController BeginPlay Call"));
	CheckClientReady();
}

void AHBVillagePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!IsLocalController())
	{
		return;
	}
	
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (PlayerIMC)
			{
				Subsystem->AddMappingContext(PlayerIMC, 0);
			}
		}
	}
	
	// Enhanced Input 바인딩
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (ToggleMapAction)
		{
			EIC->BindAction(ToggleMapAction, ETriggerEvent::Started, this, &AHBVillagePlayerController::ToggleMapWidget);
		}
		if (ObserveNextInputAction)
		{
			EIC->BindAction(ObserveNextInputAction, ETriggerEvent::Started, this, &AHBVillagePlayerController::ObserveNext);
		}
		if (ObservePrevInputAction)
		{
			EIC->BindAction(ObservePrevInputAction, ETriggerEvent::Started, this, &AHBVillagePlayerController::ObservePrev);
		}
	}

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	SetShowMouseCursor(false);
}



void AHBVillagePlayerController::CheckClientReady()
{
	HB_LOG(LogTemp, Log, TEXT("CheckClientReady Call"));
	
	bool IsGameStateReady = (Cast<AHBMafiaGameState>(GetWorld()->GetGameState()) != nullptr);
	bool IsPlayerStateReady = (PlayerState != nullptr);
	bool IsPawnReady = (Cast<AHBCharacterPlayer>(GetPawn()) != nullptr);

	if (IsGameStateReady && IsPlayerStateReady && IsPawnReady)
	{
		StartGameWidget();
		NotifyClientReady();
		HB_LOG(LogTemp, Log, TEXT("NotifyClientReady Call"));
	}
	else
	{
		FTimerHandle CheckReadyTimer;
		GetWorldTimerManager().SetTimer(CheckReadyTimer, this, &AHBVillagePlayerController::CheckClientReady, 0.3f, false);
	}
}

void AHBVillagePlayerController::NotifyClientReady_Implementation()
{
	HB_LOG(LogTemp, Log, TEXT("NotifyClientReady_Implementation Call"));
	
	AHBVillageGameMode* VillageGameMode = Cast<AHBVillageGameMode>(GetWorld()->GetAuthGameMode());
	if (!VillageGameMode) return;
	
	VillageGameMode->ApplyClientReady();
}

void AHBVillagePlayerController::StartGameWidget()
{
	InGameHUDComponent->ActivateHUD(this);
	MinimapWidgetComponent->CreateMinimapWidget(this);
	MapWidgetComponent->CreateMapWidget(this);
}

void AHBVillagePlayerController::ShowPlayerJobInfo(AHBPlayerState* TargetPlayer)
{
	JobInfoWidgetComponent->ShowJobInfoWidget(this);
}

void AHBVillagePlayerController::EnterObserveMode()
{
	if (!IsLocalPlayerController())
		return;
	bIsObserving = true;
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (PlayerIMC)
				Subsystem->RemoveMappingContext(PlayerIMC);
			if (ObserveIMC)
				Subsystem->AddMappingContext(ObserveIMC, 100);
		}
	}
	//첫 관전 대상 세팅
	ObserveToAnyAlivePlayer();
}

void AHBVillagePlayerController::ExitObserveMode()
{
	if (!IsLocalPlayerController())
		return;
	bIsObserving = false;
	ObserveCandidates.Reset();
	ObserveIndex = INDEX_NONE;

	SetIgnoreMoveInput(false);

	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsys =
			LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (ObserveIMC) Subsys->RemoveMappingContext(ObserveIMC);
			if (PlayerIMC)  Subsys->AddMappingContext(PlayerIMC, 0);
		}
	}
}

void AHBVillagePlayerController::ToggleMapWidget()
{
	if (!MapWidgetComponent->IsMapValid()) return;
	
	if (MapWidgetComponent->IsMapVisible())
	{
		MapWidgetComponent->HideMapWidget();

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(false);	
	}
	else
	{
		MapWidgetComponent->ShowMapWidget();

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		
		SetInputMode(InputMode);
		SetShowMouseCursor(true);	
	}
}

AHBCharacterPlayer* AHBVillagePlayerController::FindAnyAlivePlayer(UWorld* World, AActor* ExcludeActor)
{
	if (!World) return nullptr;

	for (TActorIterator<AHBCharacterPlayer> It(World); It; ++It)
	{
		AHBCharacterPlayer* Candidate = *It;   // 이름 변경
		if (!Candidate || Candidate == ExcludeActor) continue;

		UHBPlayerStatComponent* Stat = Candidate->FindComponentByClass<UHBPlayerStatComponent>();
		if (Stat && Stat->GetIsAlive())
		{
			return Candidate;
		}
	}

	return nullptr;
}

void AHBVillagePlayerController::ObserveToAnyAlivePlayer()
{
	if (!IsLocalController())
		return;

	BuildObserveCandidates();

	if (ObserveCandidates.Num() > 0)
	{
		ApplyObserveTarget(0, true);
		return;
	}

	// 모두 죽은 경우
	APawn* MyPawn = GetPawn();

	//살아있는 플레이어 찾아서
	AHBCharacterPlayer* Target = FindAnyAlivePlayer(GetWorld(), MyPawn);

	if (Target)
	{
		//카메라 변경
		SetViewTargetWithBlend(Target, 0.35f);
		//따로 IgnoreInput 처리가 필요 없음(실행해보니 문제 없음)
		//SetIgnoreMoveInput(true);
	}
}

void AHBVillagePlayerController::BuildObserveCandidates()
{
	ObserveCandidates.Reset();
	
	UWorld* World = GetWorld();
	if (!World)
		return;
	APawn* MyPawn = GetPawn();

	//world에 있는 캐릭터 순회
	for (TActorIterator<AHBCharacterPlayer> It(World); It; ++It)
	{
		AHBCharacterPlayer* Candidate = *It;
		if (!Candidate || Candidate == MyPawn)
		{
			continue;
		}

		UHBPlayerStatComponent* Stat = Candidate->FindComponentByClass<UHBPlayerStatComponent>();

		if (Stat && Stat->GetIsAlive())
		{
			ObserveCandidates.Add(Candidate);
		}
	}
}

void AHBVillagePlayerController::ApplyObserveTarget(int32 NewIndex, bool bBlend)
{
	if (!IsLocalController())
		return;

	if((!ObserveCandidates.IsValidIndex(NewIndex)))
		return;

	AHBCharacterPlayer* Target = ObserveCandidates[NewIndex].Get();
	if (!Target)
		return;

	ObserveIndex = NewIndex;

	SetViewTargetWithBlend(Target, 0.f);
}

void AHBVillagePlayerController::ObserveNext()
{
	if (!bIsObserving)
		return;
	BuildObserveCandidates();
	if (ObserveCandidates.Num() == 0)
		return;

	//인덱스가 순환하도록 처리 (음수가 되지 않도록 % 뒤에 .Num()사용)
	ObserveIndex = (ObserveIndex == INDEX_NONE)
		? 0 : (ObserveIndex + 1) % ObserveCandidates.Num();

	ApplyObserveTarget(ObserveIndex, true);
}

void AHBVillagePlayerController::ObservePrev()
{
	if (!bIsObserving)
		return;
	BuildObserveCandidates();
	if (ObserveCandidates.Num() == 0)
		return;

	//인덱스가 순환하도록 처리
	ObserveIndex = (ObserveIndex == INDEX_NONE)
		? 0 : (ObserveIndex - 1 + ObserveCandidates.Num()) % ObserveCandidates.Num();

	ApplyObserveTarget(ObserveIndex, true);
}
