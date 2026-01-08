// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/Job/HBJobBaseComponent.h"

#include "Character/HBCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameState/HBMafiaGameState.h"
#include "Net/UnrealNetwork.h"
#include "Prop/HBHouse.h"

// Sets default values for this component's properties
UHBJobBaseComponent::UHBJobBaseComponent()
{
	SetIsReplicatedByDefault(true);
	bIsActionActive = true;
}

void UHBJobBaseComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHBJobBaseComponent, bIsActionActive);
}

void UHBJobBaseComponent::OnRep_IsActionActive()
{
	
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

void UHBJobBaseComponent::NightPhaseEnd()
{
}

void UHBJobBaseComponent::Attack(AActor* HitActor)
{
}

void UHBJobBaseComponent::Action()
{
}

// 아직 사용하시면 안됩니다
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
	// 집 알아오기
	AHBCharacterPlayer* CharacterPlayer = GetOwner<AHBCharacterPlayer>();
	if (!CharacterPlayer)
	{
		return nullptr;
	}
	
	TArray<AActor*> OverlappingActors;
	CharacterPlayer->GetCapsuleComponent()->GetOverlappingActors(OverlappingActors, AHBHouse::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		AHBHouse* House = Cast<AHBHouse>(Actor);
		if (House)
		{
			return  House;
			break;
		}
	}
	
	return nullptr;
}

EGamePhase UHBJobBaseComponent::GetCurrentPhase()
{
	AHBMafiaGameState* GameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	ensure(GameState);

	return GameState->CurrentPhase;
}
