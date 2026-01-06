// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/Job/HBJobCelebrityComponent.h"

#include "Character/HBCharacterPlayer.h"
UHBJobCelebrityComponent::UHBJobCelebrityComponent()
{
}

void UHBJobCelebrityComponent::NightPhaseBegin()
{
	Super::NightPhaseBegin();
	UE_LOG(LogTemp, Log, TEXT("Try Cast"));
	AHBCharacterPlayer* HBCharacterPlayer = GetOwner<AHBCharacterPlayer>();
	if (!HBCharacterPlayer->DynamicMaterial)
	{
		UE_LOG(LogTemp, Log, TEXT("DynamicMaterial is NULL"));
		return;
	}
	if (HBCharacterPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("Try Celebrity"));
		HBCharacterPlayer->DynamicMaterial->SetVectorParameterValue(TEXT("CharacterBaseColor"), HBCharacterPlayer->PlayerColor);
	}

}
