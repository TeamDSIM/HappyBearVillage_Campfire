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


UCLASS()
class HAPPYBEARVILLAGE_API UHBGameVoteSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UHBGameVoteSubsystem();
	
public:
	void CalculateTop3DamagePlayers();

	void NotifyDamageChanged();


private:
	bool IsServer(UWorld* World);
	
protected:
	uint8 bRankingDirty : 1;
	uint8 bUpdateSchduled : 1;

	void UpdateRanking();
};
