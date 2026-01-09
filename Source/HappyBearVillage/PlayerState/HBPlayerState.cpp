// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/HBPlayerState.h"

#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "Net/UnrealNetwork.h"

void AHBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHBPlayerState, TotalTakenDamaged);
	DOREPLIFETIME(AHBPlayerState, UserID);
	DOREPLIFETIME(AHBPlayerState, InGameCharacterData);
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
		
	}
}

void AHBPlayerState::ResetCharacterRole()
{
	if (HasAuthority())
	{
		InGameCharacterData.CharacterRole.Job = EJobType::CITIZEN;
		InGameCharacterData.CharacterRole.Role = ERoleType::CITIZEN;
		OnRep_PlayerInGameData();
	}
}

void AHBPlayerState::ResetTotalTakenDamage()
{
}

void AHBPlayerState::ResetPlayerColor()
{
	if (HasAuthority())
	{
		InGameCharacterData.PlayerColor = FLinearColor::Gray;
		OnRep_PlayerInGameData();
	}
}

void AHBPlayerState::OnRep_PlayerInGameData()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (PC)
	{
		AHBCharacterPlayer* HBCharacterPlayer = Cast<AHBCharacterPlayer>(PC->GetPawn());
		if (HBCharacterPlayer)
		{
			HBCharacterPlayer->PlayerColor = InGameCharacterData.PlayerColor;
			HBCharacterPlayer->OnRep_PlayerColor();

			HBCharacterPlayer->GetStat()->SetCharacterRole(InGameCharacterData.CharacterRole);
			HBCharacterPlayer->GetStat()->OnRep_CharacterRole();
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
