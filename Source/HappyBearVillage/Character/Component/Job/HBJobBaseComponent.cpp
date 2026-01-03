// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/Job/HBJobBaseComponent.h"

// Sets default values for this component's properties
UHBJobBaseComponent::UHBJobBaseComponent()
{
	TargetPlayer = nullptr;
	TargetHouse = nullptr;

	SetIsReplicatedByDefault(true);
}

void UHBJobBaseComponent::OnGameBegin()
{
}

void UHBJobBaseComponent::OnDayPhaseBegin()
{
}

void UHBJobBaseComponent::OnNightPhaseBegin()
{
}

void UHBJobBaseComponent::OnAttack()
{
}

void UHBJobBaseComponent::Action()
{
}

AHBCharacterPlayer* UHBJobBaseComponent::DetectTarget()
{
	return nullptr;
}

AHBHouse* UHBJobBaseComponent::DetectHouse()
{
	return nullptr;
}
