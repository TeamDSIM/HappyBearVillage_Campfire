// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/Job/HBJobArmyComponent.h"

#include "Character/HBCharacterBase.h"
#include "Character/Stat/HBPlayerStatComponent.h"

UHBJobArmyComponent::UHBJobArmyComponent()
{
}

void UHBJobArmyComponent::OnNightPhaseBegin()
{
	Super::OnNightPhaseBegin();

	AHBCharacterBase* HBCharacterBase = GetOwner<AHBCharacterBase>();
	if (HBCharacterBase)
	{
		HBCharacterBase->GetStat()->SetHealth(3);
	}
}
