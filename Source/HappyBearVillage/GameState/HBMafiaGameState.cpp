// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/HBMafiaGameState.h"

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
