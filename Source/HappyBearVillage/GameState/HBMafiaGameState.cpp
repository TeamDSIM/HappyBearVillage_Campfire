// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/HBMafiaGameState.h"
#include "Character/HBCharacterPlayer.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/HBVillageGenerationWorldSubsystem.h"


AHBMafiaGameState::AHBMafiaGameState()
{
	GameEnd = 0;
}

void AHBMafiaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHBMafiaGameState, GameProgress);
	DOREPLIFETIME(AHBMafiaGameState, CurrentPhase);
	DOREPLIFETIME(AHBMafiaGameState, RemainingTime);
	DOREPLIFETIME(AHBMafiaGameState, Date);
	DOREPLIFETIME(AHBMafiaGameState, TopDamagePlayers);
	DOREPLIFETIME(AHBMafiaGameState, TargetVoteNum);
	DOREPLIFETIME(AHBMafiaGameState, GameEnd);
}

void AHBMafiaGameState::SetTopDamagePlayers(const TArray<FDamageRankEntry>& NewTopDamagePlayers)
{
	if (HasAuthority())
	{
		TopDamagePlayers = NewTopDamagePlayers;

		OnRep_TopDamagePlayers();
	}
}

AHBCharacterPlayer* AHBMafiaGameState::GetPlayerByColor(FLinearColor InColor)
{
	for (APlayerState* PS : PlayerArray)
	{
		AController* PlayerController = PS->GetPlayerController();
		if (PlayerController)
		{
			AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(PlayerController->GetPawn());
			if (Character && Character->PlayerColor == InColor)
			{
				return Character;
			}
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Player not found"));
	return nullptr;
}

void AHBMafiaGameState::OnRep_TopDamagePlayers()
{
	for (int i = 0; i < TopDamagePlayers.Num(); ++i)
	{
		OnTopDamagePlayersChanged.Broadcast(TopDamagePlayers[i].PlayerState, TopDamagePlayers[i], i);
	}
}

void AHBMafiaGameState::OnRep_GameProgress()
{
	OnGameProgressChanged.Broadcast(GameProgress);
}

void AHBMafiaGameState::OnRep_GamePhase()
{
	OnGamePhaseChanged.Broadcast(CurrentPhase);
	
	if (CurrentPhase == EGamePhase::VoteCheck)
	{
		if (GameEnd == 0)
		{
			OnFadeAnimationPlay.Broadcast(true);
		}
	}
	
	if (CurrentPhase == EGamePhase::Night)
	{
		if (GameEnd == 0)
		{
			OnFadeAnimationPlay.Broadcast(false);
		}
	}
}

void AHBMafiaGameState::OnRep_RemainingTime()
{
	OnRemainingTimeChanged.Broadcast(RemainingTime);
}

void AHBMafiaGameState::OnRep_Date()
{
	OnDateChanged.Broadcast(Date);
}

void AHBMafiaGameState::OnRep_TargetVoteNum()
{
	OnTargetVoteNumChanged.Broadcast(TargetVoteNum);
}

void AHBMafiaGameState::OnRep_GameEnd()
{
	OnGameEndChanged.Broadcast(GameEnd);
}

void AHBMafiaGameState::OnRep_MapMarks()
{
	OnMapMarksChanged.Broadcast(MapMarks);
}

bool AHBMafiaGameState::IsNight() const
{
	return CurrentPhase == EGamePhase::Night;
}

void AHBMafiaGameState::OnRep_VillageGenerationData()
{
	UHBVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UHBVillageGenerationWorldSubsystem>();
	if (!VillageGenerationSystem) return;
	if (VillageGenerationSystem->IsGenerated()) return;
	
	VillageGenerationSystem->GenerateVillage(VillageGenerationData);
}

//LobbyWidget ���� �ڵ��Դϴ�.
void AHBMafiaGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	OnLobbyPlayersChanged.Broadcast();
	OnPlayerStateArrayChanged.Broadcast();
}

void AHBMafiaGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	OnLobbyPlayersChanged.Broadcast();
}