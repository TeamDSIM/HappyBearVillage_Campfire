// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	void SyncTotalTakenDamagedFromPlayerStat(float NewDamage);
	void SyncPlayerColorFromPlayerStat(FLinearColor NewColor);
	void SetUserID(FString NewID);

	FOnDamageChanged OnDamageChanged;

protected:
	UPROPERTY(Replicated)
	float TotalTakenDamaged = 0;

	UPROPERTY(Replicated)
	FLinearColor PlayerColor = FColor::White;

	UPROPERTY(Replicated)
	FString UserID;
};
