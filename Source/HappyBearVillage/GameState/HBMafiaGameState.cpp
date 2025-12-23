// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/HBMafiaGameState.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

AHBMafiaGameState::AHBMafiaGameState()
{
	
}

void AHBMafiaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHBMafiaGameState, CurrentPhase);
	DOREPLIFETIME(AHBMafiaGameState, RemainingTime);
}

void AHBMafiaGameState::OnRep_GamePhase()
{
	OnGamePhaseChanged.Broadcast(CurrentPhase);
}

void AHBMafiaGameState::OnRep_RemainingTime()
{
	OnRemainingTimeChanged.Broadcast(RemainingTime);
}

bool AHBMafiaGameState::IsNight() const
{
	return CurrentPhase == EGamePhase::Night;
}

//LobbyWidget 관련 코드입니다.
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
