// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Stat/HBPlayerStatComponent.h"

#include "HappyBearVillage.h"
#include "Character/Component/HBCharacterRagdollComponent.h"
#include "Character/Component/Job/HBJobArmyComponent.h"
#include "Character/Component/Job/HBJobAssassinComponent.h"
#include "Editor/WidgetCompilerLog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameState/HBMafiaGameState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/HBPlayerState.h"
#include "Subsystem/HBGameVoteSubsystem.h"
#include "Controller/HBPlayerController.h"


// Sets default values for this component's properties
UHBPlayerStatComponent::UHBPlayerStatComponent()
{
	bWantsInitializeComponent = true;
	bIsVoteTarget = false;
	bIsAlive = true;

	SetIsReplicatedByDefault(true);
}

void UHBPlayerStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}


// Called when the game starts
void UHBPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

float UHBPlayerStatComponent::ApplyDamage(float InDamageAmount)
{
	TotalTakenDamage += InDamageAmount;
	OnTotalTakenDamageChanged.Broadcast(TotalTakenDamage);

	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn)
	{
		AHBPlayerState* PawnState = Cast<AHBPlayerState>(Pawn->GetPlayerState());
		if (PawnState)
		{
			PawnState->SyncTotalTakenDamagedFromPlayerStat(TotalTakenDamage);
			UHBGameVoteSubsystem* VoteSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UHBGameVoteSubsystem>();
			if (VoteSubsystem)
			{
				VoteSubsystem->NotifyDamageChanged();
			}
		}
	}

	return TotalTakenDamage;
}

int32 UHBPlayerStatComponent::ApplyNightDamage()
{
	if (Health <= 0)
	{
		return 0;
	}
	
	Health -= 1;
	
	if (Health <= 0)
	{
		Health = 0;
		OnHealthChanged.Broadcast(Health);

		SetIsAlive(false);
	}

	return Health;
}

void UHBPlayerStatComponent::ResetTotalTakenDamage()
{
	TotalTakenDamage = 0;

	OnRep_TotalTakenDamage();
}

void UHBPlayerStatComponent::InitCharacterRole()
{
	// 랜덤 직업 설정
	CharacterRole.InitRole();

	// 바뀐 직업 서버 반영
	OnRep_CharacterRole();
}

void UHBPlayerStatComponent::InitCharacterRole(EJobType InJob)
{
	// 랜덤 직업 설정
	UE_LOG(LogTemp, Log, TEXT(" PlayerJobs : %d"), static_cast<int32>(InJob));
	CharacterRole.InitRole(InJob);

	AHBCharacterPlayer* HBCharacterPlayer = Cast<AHBCharacterPlayer>(GetOwner());
	if (HBCharacterPlayer)
	{
		switch (InJob)
		{
		case EJobType::MAFIA:
			break;
		case EJobType::SHACO:
			{
				HBCharacterPlayer->AssignJob(UHBJobAssassinComponent::StaticClass());
			}
			break;
		case EJobType::CITIZEN:
			break;
		case EJobType::ARMY:
			{
				HBCharacterPlayer->AssignJob(UHBJobArmyComponent::StaticClass());
			}
			break;
		default:
			break;
		}
	}

	// 바뀐 직업 서버 반영
	OnRep_CharacterRole();
}

void UHBPlayerStatComponent::ResetCharacterRole()
{
	CharacterRole.Job = EJobType::CITIZEN;
	CharacterRole.Role = ERoleType::CITIZEN;

	OnRep_CharacterRole();
}

void UHBPlayerStatComponent::ApplyVote(AActor* InActor)
{
	// 투표 대상이 아니라면 반환
	if (!bIsVoteTarget)
	{
		return;
	}

	// 때린 플레이어가 이미 투표한 상태라면 반환
	if (VotedPlayers.Contains(InActor))
	{
		return;
	}

	VotedPlayers.Add(InActor);
	VoteNum += 1;

	OnRep_VoteNum();

	HB_SUBLOG(LogTemp, Log, TEXT("VoteNum : %d"), VoteNum);
}

void UHBPlayerStatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHBPlayerStatComponent, TotalTakenDamage);
	DOREPLIFETIME(UHBPlayerStatComponent, Health);
	DOREPLIFETIME(UHBPlayerStatComponent, CharacterRole);
	DOREPLIFETIME(UHBPlayerStatComponent, VoteNum);
	DOREPLIFETIME(UHBPlayerStatComponent, bIsVoteTarget);
	DOREPLIFETIME(UHBPlayerStatComponent, bIsAlive);
}

void UHBPlayerStatComponent::OnRep_TotalTakenDamage()
{
	OnTotalTakenDamageChanged.Broadcast(TotalTakenDamage);
}

void UHBPlayerStatComponent::OnRep_CharacterRole()
{
	// UI 갱신 시켜야 함
	OnPlayerRoleChanged.Broadcast(CharacterRole.Role);
	OnPlayerJobChanged.Broadcast(CharacterRole.Job);
}

void UHBPlayerStatComponent::OnRep_VoteNum()
{
	if (bIsVoteTarget && bIsAlive)
	{
		AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
		if (HBGameState)
		{
			UE_LOG(LogTemp, Log, TEXT("OnRep_VoteNum = %d"), VoteNum);
			HBGameState->TargetVoteNum = VoteNum;

			HBGameState->OnRep_TargetVoteNum();
			HBGameState->TargetVoteNum = -1;
		}
	}
}

void UHBPlayerStatComponent::OnRep_Health()
{
}

void UHBPlayerStatComponent::OnRep_IsAlive()
{
	AHBCharacterPlayer* CharacterPlayer = Cast<AHBCharacterPlayer>(GetOwner());
	if (CharacterPlayer)
	{
		if (!bIsAlive)
		{
			CharacterPlayer->GetComponentByClass<UHBCharacterRagdollComponent>()->ApplyRagdoll();
			CharacterPlayer->GetCharacterMovement()->DisableMovement();
			CharacterPlayer->GetCharacterMovement()->StopMovementImmediately();
			if (CharacterPlayer->IsLocallyControlled())
			{
				if (AHBPlayerController* PC =
					Cast<AHBPlayerController>(CharacterPlayer->GetController()))
				{
					PC->EnterObserveMode();
				}
			}
		}
	}
}

void UHBPlayerStatComponent::ClearVotedInfo()
{
	VotedPlayers.Empty();
	VoteNum = 0;
}
