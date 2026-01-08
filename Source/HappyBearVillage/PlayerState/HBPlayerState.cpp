// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/HBPlayerState.h"

#include "Character/HBCharacterPlayer.h"
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

void AHBPlayerState::OnRep_PlayerColor()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (PC)
	{
		AHBCharacterPlayer* HBCharacterPlayer = Cast<AHBCharacterPlayer>(PC->GetPawn());
		if (HBCharacterPlayer)
		{
			HBCharacterPlayer->PlayerColor = this->PlayerColor;
			HBCharacterPlayer->OnRep_PlayerColor();
		}
	}
}

void AHBPlayerState::SetUserID(FString NewID)
{
	if (HasAuthority())
	{
		UserID = NewID;
	}
}
