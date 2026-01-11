// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Component/Job/HBJobSantaComponent.h"
#include "Character/HBCharacterPlayer.h"
#include "GameState/HBMafiaGameState.h"
#include "PlayerState/HBPlayerState.h"
#include "Prop/HBPresent.h"
#include "Prop/HBHouse.h"


UHBJobSantaComponent::UHBJobSantaComponent()
{
	PresentClass = AHBPresent::StaticClass();
}

void UHBJobSantaComponent::NightPhaseBegin()
{
	Super::NightPhaseBegin();

	bUseAbilityTonight = false;
	ApplyLastNightPresentAbility();
}

void UHBJobSantaComponent::NightPhaseEnd()
{
	LastNightPresent = TonightPresent;
	LastNightTargetColor = TonightTargetColor;

	TonightPresent = nullptr;
	TonightTargetColor = FLinearColor(0, 0, 0, 0);
}

void UHBJobSantaComponent::Action()
{
	Super::Action();
	
	if (!bIsActionActive || GetCurrentPhase() != EGamePhase::Night)
	{
		return;
	}

	SpawnPresent();
}

void UHBJobSantaComponent::SpawnPresent_Implementation()
{
	AHBHouse* TargetHouse = DetectHouse();
	if (!TargetHouse) return;
	
	UE_LOG(LogTemp, Log, TEXT("House Detected"));
	
	FLinearColor HouseColor = TargetHouse->GetHouseColor();
	if (TonightTargetColor == HouseColor || bUseAbilityTonight) return;

	TonightTargetColor = HouseColor;
	bUseAbilityTonight = true;
	
	AHBCharacterPlayer* Santa = Cast<AHBCharacterPlayer>(GetOwner());
	FVector SpawnLocation = Santa->GetActorLocation() + Santa->GetActorForwardVector() * 50.f;
	FRotator SpawnRotation = Santa->GetActorRotation();

	UE_LOG(LogTemp, Log, TEXT("Spawn Present"));
	AHBPresent* Present = GetWorld()->SpawnActor<AHBPresent>(PresentClass, SpawnLocation, SpawnRotation);
	TonightPresent = Present;
}

void UHBJobSantaComponent::ApplyLastNightPresentAbility()
{
	if (!LastNightPresent) return;

	for (APlayerState* PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		AHBPlayerState* HBPlayerState = Cast<AHBPlayerState>(PlayerState);
		AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(HBPlayerState->GetPawn());

		if (Character->PlayerColor == LastNightTargetColor)
		{
			Character->StaminaRecovery();
			UE_LOG(LogTemp, Log, TEXT("Recovery Target Stamina"));
		}
	}
	
	LastNightPresent->Destroy();
}