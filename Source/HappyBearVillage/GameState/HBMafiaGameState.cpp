// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/HBMafiaGameState.h"

#include "HappyBearVillage.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/HBPlayerState.h"
#include "ProceduralGeneration/Map/HBMapGenerator.h"
#include "ProceduralGeneration/MapData/HBMapDataGenerator.h"
#include "ProceduralGeneration/Noise/HBPerlinNoise.h"
#include "Utils/HBUtils.h"

AHBMafiaGameState::AHBMafiaGameState()
{
	
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
		HB_LOG(LogHY, Log, TEXT("ID : %d, RankEntryDamage : %.0f, PlayerStateDamage : %.0f"),
			TopDamagePlayers[i].PlayerState->GetUserID(),
			TopDamagePlayers[i].TotalTakenDamaged,
			TopDamagePlayers[i].PlayerState->GetTotalTakenDamaged()
			);
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

bool AHBMafiaGameState::IsNight() const
{
	return CurrentPhase == EGamePhase::Night;
}

void AHBMafiaGameState::OnRep_VillageGenerationSyncData()
{
	HBUtils::InitRandomSeed(VillageGenerationSyncData.RandomStreamSeed);

	UHBPerlinNoise* PerlinNoise = NewObject<UHBPerlinNoise>();
	UHBMapDataGenerator* MapDataGenerator = NewObject<UHBMapDataGenerator>();
	UHBMapGenerator* MapGenerator = NewObject<UHBMapGenerator>();

	PerlinNoise->GeneratePerlinNoise(VillageGenerationSyncData.NoiseSettings);
	MapDataGenerator->GenerateFieldData(PerlinNoise);
	MapDataGenerator->GenerateHouseData(8);
	MapDataGenerator->GenerateForestData();
	MapDataGenerator->GenerateForestTexture2D();
	MapDataGenerator->UpdateMap();

	FHBMapData MapData = MapDataGenerator->GetMapData();

	MapGenerator->GenerateField(MapData, GetWorld());
	MapGenerator->GenerateHouse(MapData, GetWorld());
	MapGenerator->GenerateForestSpline(MapData, GetWorld());
	MapGenerator->GenerateVillage(MapData, GetWorld());
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