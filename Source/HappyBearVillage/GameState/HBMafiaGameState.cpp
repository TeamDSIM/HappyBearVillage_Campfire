// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/HBMafiaGameState.h"

#include "EngineUtils.h"
#include "HappyBearVillage.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/HBPlayerState.h"
#include "ProceduralGeneration/Map/HBMapGenerator.h"
#include "ProceduralGeneration/MapData/HBMapDataGenerator.h"
#include "ProceduralGeneration/Noise/HBPerlinNoise.h"
#include "Subsystem/HBVillageGenerationWorldSubsystem.h"
#include "Utils/HBUtils.h"

AHBMafiaGameState::AHBMafiaGameState()
{
	GameEnd = 0;
}

void AHBMafiaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

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

	const bool bIsNight = (CurrentPhase == EGamePhase::Night);
	for (TActorIterator<AHBCharacterPlayer> It(GetWorld()); It; ++It)
	{
		It->ApplyNightColor(bIsNight);
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
	UE_LOG(LogTemp, Log, TEXT("OnRep_TargetVoteNum"));
	OnTargetVoteNumChanged.Broadcast(TargetVoteNum);
}

void AHBMafiaGameState::OnRep_GameEnd()
{
	OnGameEndChanged.Broadcast(GameEnd);
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
}

void AHBMafiaGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	OnLobbyPlayersChanged.Broadcast();
}