// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/HBPlayerState.h"

#include "Net/UnrealNetwork.h"

void AHBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHBPlayerState, TotalTakenDamaged);
	DOREPLIFETIME(AHBPlayerState, PlayerColor);
	DOREPLIFETIME(AHBPlayerState, UserID);
}

void AHBPlayerState::SyncTotalTakenDamagedFromPlayerStat(float NewDamage)
{
	if (HasAuthority())
	{
		TotalTakenDamaged = NewDamage;
		
		OnDamageChanged.Broadcast();
	}
}

void AHBPlayerState::SyncPlayerColorFromPlayerStat(FLinearColor NewColor)
{
	if (HasAuthority())
	{
		PlayerColor = NewColor;
	}
}

void AHBPlayerState::SetUserID(int32 NewID)
{
	if (HasAuthority())
	{
		UserID = NewID;
	}
}
