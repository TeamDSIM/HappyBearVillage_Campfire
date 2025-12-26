// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/HBMafiaGameState.h"

#include "HappyBearVillage.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/HBPlayerState.h"

AHBMafiaGameState::AHBMafiaGameState()
{
	
}

void AHBMafiaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHBMafiaGameState, CurrentPhase);
	DOREPLIFETIME(AHBMafiaGameState, RemainingTime);
	DOREPLIFETIME(AHBMafiaGameState, Date);
	DOREPLIFETIME(AHBMafiaGameState, TopDamagePlayers);
}

void AHBMafiaGameState::SetTopDamagePlayers(const TArray<FDamageRankEntry>& NewTopDamagePlayers)
{
	if (HasAuthority())
	{
		TopDamagePlayers = NewTopDamagePlayers;

		OnRep_TopDamagePlayers();
	}
}

void AHBMafiaGameState::OnRep_TopDamagePlayers()
{
	for (int i = 0; i < TopDamagePlayers.Num(); ++i)
	{
		OnTopDamagePlayersChanged.Broadcast(TopDamagePlayers[i].PlayerState, TopDamagePlayers[i], i);
	}
}

void AHBMafiaGameState::OnRep_GamePhase()
{
	OnGamePhaseChanged.Broadcast(CurrentPhase);
}

void AHBMafiaGameState::OnRep_RemainingTime()
{
	OnRemainingTimeChanged.Broadcast(RemainingTime);
}

void AHBMafiaGameState::OnRep_Date()
{
	OnDateChanged.Broadcast(Date);
}

bool AHBMafiaGameState::IsNight() const
{
	return CurrentPhase == EGamePhase::Night;
}

//LobbyWidget ���� �ڵ��Դϴ�.
void AHBMafiaGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	OnLobbyPlayersChanged.Broadcast();
}

void AHBMafiaGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	OnLobbyPlayersChanged.Broadcast();
}
