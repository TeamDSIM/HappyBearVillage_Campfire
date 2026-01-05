// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/Job/HBJobBaseComponent.h"

#include "GameState/HBMafiaGameState.h"

// Sets default values for this component's properties
UHBJobBaseComponent::UHBJobBaseComponent()
{
	SetIsReplicatedByDefault(true);
	bIsActionActive = true;
}

void UHBJobBaseComponent::GameBegin()
{
}

void UHBJobBaseComponent::DayPhaseBegin()
{
}

void UHBJobBaseComponent::NightPhaseBegin()
{
}

void UHBJobBaseComponent::Attack(AActor* HitActor)
{
}

void UHBJobBaseComponent::Action()
{
}

AHBCharacterPlayer* UHBJobBaseComponent::DetectTarget()
{
	return nullptr;
}

TArray<AHBCharacterPlayer*> UHBJobBaseComponent::DetectTargets()
{
	return TArray<AHBCharacterPlayer*>();
}

AHBHouse* UHBJobBaseComponent::DetectHouse()
{
	return nullptr;
}

EGamePhase UHBJobBaseComponent::GetCurrentPhase()
{
	AHBMafiaGameState* GameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	ensure(GameState);

	return GameState->CurrentPhase;
}
