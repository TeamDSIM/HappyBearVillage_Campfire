// Fill out your copyright notice in the Description page of Project Settings.


#include "HBJobDetectorComponent.h"

#include <HBPlayerRoleWidget.generated.h>

#include "Character/HBCharacterPlayer.h"
#include "Controller/HBVillagePlayerController.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState/HBPlayerState.h"
#include "Prop/HBHouse.h"


UHBJobDetectorComponent::UHBJobDetectorComponent()
{

}

void UHBJobDetectorComponent::NightPhaseBegin()
{
	Super::NightPhaseBegin();

	bUseAbilityTonight = false;
}

void UHBJobDetectorComponent::Action()
{
	Super::Action();

	if (!bIsActionActive || GetCurrentPhase() != EGamePhase::Night)
	{
		return;
	}

	ShowTargetJob();
}

void UHBJobDetectorComponent::ShowTargetJob()
{
	AHBHouse* TargetHouse = DetectHouse();
	if (!TargetHouse) return;

	UE_LOG(LogTemp, Log, TEXT("House Detected"));

	FLinearColor TargetColor = TargetHouse->GetHouseColor();
	if (bUseAbilityTonight) return;
	
	bUseAbilityTonight = true;

	for (APlayerState* PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		AHBPlayerState* HBPlayerState = Cast<AHBPlayerState>(PlayerState);
		if (HBPlayerState->GetPlayerColor() == TargetColor)
		{
			EJobType Job = HBPlayerState->GetCharacterRole().Job;

			AHBVillagePlayerController* VillagePlayerController = Cast<AHBVillagePlayerController>(GetWorld()->GetFirstPlayerController());
			VillagePlayerController->ShowPlayerJobInfo(HBPlayerState);
		}
	}
}