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
}
