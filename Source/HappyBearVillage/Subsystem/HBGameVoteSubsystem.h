// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/HBCharacterPlayer.h"
#include "PlayerState/HBPlayerState.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HBGameVoteSubsystem.generated.h"

/**
 * 
 */


class AHBExecutePlatform;

UCLASS()
class HAPPYBEARVILLAGE_API UHBGameVoteSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UHBGameVoteSubsystem();

public:
	void CalculateTop3DamagePlayers();

	void NotifyDamageChanged();

	void ClearCurrentVoteTarget() { CurrentVoteTarget = nullptr; }
	FORCEINLINE void SetCurrentVoteTarget(AHBCharacterPlayer* InHBCharacterPlayer)
	{
		CurrentVoteTarget = InHBCharacterPlayer;
	}

	FORCEINLINE AHBCharacterPlayer* GetCurrentVoteTarget() { return CurrentVoteTarget; }

	// 투표 과반수 확인
	void CheckTargetIsDead();

	FORCEINLINE AHBExecutePlatform* GetExecutePlatform() const { return ExecutePlatform; }
	FORCEINLINE void SetExecutePlatform(AHBExecutePlatform* InExecutePlatform)
	{
		ExecutePlatform = InExecutePlatform;
	}

	FORCEINLINE void ClearExecutePlatform() { ExecutePlatform = nullptr; }

private:
	bool IsServer(UWorld* World);

protected:
	uint8 bRankingDirty : 1;
	uint8 bUpdateSchduled : 1;

	void UpdateRanking();

private:
	// 이번 투표 대상
	AHBCharacterPlayer* CurrentVoteTarget;

	UPROPERTY()
	TObjectPtr<AHBExecutePlatform> ExecutePlatform;
};
