// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HBInGameCharacterData.h"
#include "Character/Stat/HBCharacterRole.h"
#include "GameFramework/PlayerState.h"
#include "HBPlayerState.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnDamageChanged);

UCLASS()
class HAPPYBEARVILLAGE_API AHBPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FORCEINLINE float GetTotalTakenDamaged() const { return TotalTakenDamaged; }
	FORCEINLINE FLinearColor GetPlayerColor() const { return InGameCharacterData.PlayerColor; }
	FORCEINLINE FString GetUserID() const { return UserID; }
	FORCEINLINE FHBCharacterRole GetCharacterRole() const { return InGameCharacterData.CharacterRole; }

	FORCEINLINE void SetPlayerColor(FLinearColor NewColor)
	{
		if (HasAuthority())
		{
			InGameCharacterData.SetPlayerColor(NewColor);
		}
	}
	
	void SetUserID(FString NewID);
	
	FORCEINLINE void SetCharacterRole(EJobType InJob)
	{
		if (HasAuthority())
		{
			InGameCharacterData.SetCharacterRole(InJob);
		}
	}

	FORCEINLINE void SetHBInGameCharacterData(FHBInGameCharacterData NewData)
	{
		if (HasAuthority())
		{
			InGameCharacterData = NewData;
			OnRep_PlayerInGameData();
		}
	}

	void SyncTotalTakenDamagedFromPlayerStat(float NewDamage);
	void SyncPlayerColorFromPlayerStat(FLinearColor NewColor);

	void ResetCharacterRole();
	void ResetTotalTakenDamage();
	void ResetPlayerColor();

	UFUNCTION()
	void OnRep_PlayerInGameData();

	FOnDamageChanged OnDamageChanged;

protected:
	UPROPERTY(Replicated)
	float TotalTakenDamaged = 0;
	
	UPROPERTY(Replicated)
	FString UserID;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerInGameData)
	FHBInGameCharacterData InGameCharacterData;
};
