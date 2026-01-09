// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	FORCEINLINE FLinearColor GetPlayerColor() const { return PlayerColor; }
	FORCEINLINE FString GetUserID() const { return UserID; }
	FORCEINLINE FHBCharacterRole GetCharacterRole() const { return CharacterRole; }

	FORCEINLINE void SetPlayerColor(FLinearColor NewColor)
	{
		PlayerColor = NewColor;
		OnRep_PlayerColor();
	}
	void SetUserID(FString NewID);
	FORCEINLINE void SetCharacterRole(EJobType InJob)
	{
		CharacterRole.InitRole(InJob);
		OnRep_CharacterRole();
	}

	void SyncTotalTakenDamagedFromPlayerStat(float NewDamage);
	void SyncPlayerColorFromPlayerStat(FLinearColor NewColor);

	void ResetCharacterRole();
	void ResetTotalTakenDamage();
	void ResetPlayerColor();

	UFUNCTION()
	void OnRep_PlayerColor();

	UFUNCTION()
	void OnRep_CharacterRole();

	FOnDamageChanged OnDamageChanged;

protected:
	UPROPERTY(Replicated)
	float TotalTakenDamaged = 0;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerColor)
	FLinearColor PlayerColor = FLinearColor::Gray;

	UPROPERTY(Replicated)
	FString UserID;

	UPROPERTY(ReplicatedUsing = OnRep_CharacterRole)
	FHBCharacterRole CharacterRole;
};
