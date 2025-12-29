// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/HBCharacterPlayer.h"
#include "GameFramework/GameStateBase.h"
#include "ProceduralGeneration/MapData/HBMapData.h"
#include "ProceduralGeneration/Sync/HBVillageGenerationData.h"
#include "HBMafiaGameState.generated.h"

/**
 * 
 */

class AHBPlayerState;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGamePhaseChanged, EGamePhase)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemainingTimeChanged, float /*RemainingTime*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDateChanged, int32 /*Date*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetVoteNumChanged, int32 /*TargetVoteNum*/)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnTopDamagePlayersChanged, AHBPlayerState*, FDamageRankEntry, int32)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameEndChanged, int32);


//LobbyWidget 관련 코드입니다.
DECLARE_MULTICAST_DELEGATE(FOnLobbyPlayersChanged);

// 각 페이즈를 나타내는 enum 클래스
UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	Day UMETA(DisplayName = "Day"),	// 대기 상태 (낮)
	Discussion UMETA(DisplayName = "Discussion"),	// 토론 상태
	Fight UMETA(DisplayName = "Fight"),		// 난투 상태
	Vote UMETA(DisplayName = "Vote"),		// 최종 투표 상태
	Night UMETA(DisplayName = "Night"),		// 밤 상태


	Lobby UMETA(DisplayName = "Lobby"),
	End UMETA(DisplayName = "End"),
};

FORCEINLINE EGamePhase operator+(EGamePhase Phase, int32 Offset)
{
	const int32 Value = static_cast<int32>(Phase);
	const int32 Max   = static_cast<int32>(EGamePhase::Lobby);

	int32 Next = (Value + Offset) % Max;
	return static_cast<EGamePhase>(Next);
}

USTRUCT(BlueprintType)
struct FDamageRankEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AHBPlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly)
	float TotalTakenDamaged;
};


UCLASS()
class HAPPYBEARVILLAGE_API AHBMafiaGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AHBMafiaGameState();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FOnGamePhaseChanged OnGamePhaseChanged;
	FOnRemainingTimeChanged OnRemainingTimeChanged;
	FOnDateChanged OnDateChanged;
	FOnTopDamagePlayersChanged OnTopDamagePlayersChanged;
	FOnTargetVoteNumChanged OnTargetVoteNumChanged;
	FOnGameEndChanged OnGameEndChanged;

public:
	void SetTopDamagePlayers(const TArray<FDamageRankEntry>& NewTopDamagePlayers);
	
public:
	// @PHYTODO : 나중에 다 컴포넌트로 생성해서 옮겨주기
	UPROPERTY(ReplicatedUsing = OnRep_GamePhase, BlueprintReadOnly)
	EGamePhase CurrentPhase = EGamePhase::Lobby;

	UPROPERTY(ReplicatedUsing = OnRep_RemainingTime, BlueprintReadOnly)
	float RemainingTime = 0.f;

	// 현재 플레이중인 일차
	UPROPERTY(ReplicatedUsing = OnRep_Date, BlueprintReadOnly)
	int32 Date = 0;

	// Top3 데미지 유저 출력용
	UPROPERTY(ReplicatedUsing = OnRep_TopDamagePlayers)
	TArray<FDamageRankEntry> TopDamagePlayers;

	UPROPERTY(ReplicatedUsing = OnRep_TargetVoteNum)
	int32 TargetVoteNum = -1;

	// 0 은 종료 X, 1 은 마피아, 2 는 시민 승리
	UPROPERTY(ReplicatedUsing = OnRep_GameEnd)
	int32 GameEnd = 0;

	UFUNCTION()
	void OnRep_TopDamagePlayers();

	UFUNCTION()
	void OnRep_GamePhase();

	UFUNCTION()
	void OnRep_RemainingTime();

	UFUNCTION()
	void OnRep_Date();

	UFUNCTION()
	void OnRep_TargetVoteNum();

	UFUNCTION()
	void OnRep_GameEnd();
	
	UFUNCTION(BlueprintCallable)
	bool IsNight() const;

	//LobbyWidget 관련 코드입니다.
public:
	// LobbyWidget이 구독할 이벤트
	FOnLobbyPlayersChanged OnLobbyPlayersChanged;

	UFUNCTION()
	void OnRep_VillageGenerationData();

protected:
	// PlayerArray가 변하는 시점(추가/삭제)에 엔진이 호출
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

public:
	UPROPERTY(ReplicatedUsing = OnRep_VillageGenerationData)
	FHBVillageGenerationData VillageGenerationData;
};
