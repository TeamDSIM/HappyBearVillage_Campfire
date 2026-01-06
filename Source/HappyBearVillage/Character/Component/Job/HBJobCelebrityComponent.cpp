// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/Job/HBJobCelebrityComponent.h"

#include "Character/HBCharacterPlayer.h"
UHBJobCelebrityComponent::UHBJobCelebrityComponent()
{
}

void UHBJobCelebrityComponent::NightPhaseBegin()
{
	Super::NightPhaseBegin();

	AHBCharacterPlayer* HBCharacterPlayer = GetOwner<AHBCharacterPlayer>();

	if (HBCharacterPlayer)
	{
		if (HBCharacterPlayer->HasAuthority())
		{
			FLinearColor PlayerColor = HBCharacterPlayer->PlayerColor;
			HBCharacterPlayer->RenderColor = PlayerColor;
			HBCharacterPlayer->OnRep_RenderColor();
		}
	}
}
