// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/HBMafiaGameState.h"

#include "HappyBearVillage.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/HBPlayerState.h"

AHBMafiaGameState::AHBMafiaGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHBMafiaGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Timersss -= DeltaSeconds;
	if (Timersss <= 0.0f)
	{
		for (int i = 0; i < TopDamagePlayers.Num(); ++i)
		{
			AC_LOG(LogHY, Error, TEXT("%.0f"), TopDamagePlayers[i].TotalTakenDamaged);
		}
		Timersss += 1.0f;
	}
}

void AHBMafiaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHBMafiaGameState, CurrentPhase);
	DOREPLIFETIME(AHBMafiaGameState, RemainingTime);
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
		UE_LOG(LogTemp, Log, TEXT("TopDamagePlayers %d : %d"), i, TopDamagePlayers[i].PlayerState->GetPlayerId());
		UE_LOG(LogTemp, Log, TEXT("TOpDamage %.0f"),TopDamagePlayers[i].TotalTakenDamaged);
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
