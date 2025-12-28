// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Component/HBCharacterRelocationComponent.h"

#include "HappyBearVillage.h"
#include "GameFramework/PlayerState.h"
#include "GameState/HBMafiaGameState.h"
#include "Character/HBCharacterPlayer.h"
#include "Subsystem/VillageGenerationWorldSubsystem.h"

UHBCharacterRelocationComponent::UHBCharacterRelocationComponent() { }

void UHBCharacterRelocationComponent::RelocateCharactersToHouse(AHBMafiaGameState* InGameState)
{
	if (!InGameState) return;
	
	UVillageGenerationWorldSubsystem* VillageGenerationSystem = GetWorld()->GetSubsystem<UVillageGenerationWorldSubsystem>();
	if (!VillageGenerationSystem) return;

	const TMap<FLinearColor, FVector>& HouseLocationsByColor = VillageGenerationSystem->GetMapData().HouseLocationsByColor;
	
	for (APlayerState* Player : InGameState->PlayerArray)
	{
		AController* PlayerContoller = Player->GetPlayerController();
		if (!PlayerContoller)
		{
			UE_LOG(LogYS, Log, TEXT("Failed Character Relocate To House - 1"));
			return;
		}

		AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(PlayerContoller->GetPawn());
		if (!Character)
		{
			UE_LOG(LogYS, Log, TEXT("Failed Character Relocate To House - 2"));
			return;
		}

		FLinearColor CharacterColor = Character->PlayerColor;
		FVector HouseLocation = HouseLocationsByColor[CharacterColor];
		HouseLocation = FVector(HouseLocation.X * 400, HouseLocation.Y * 400, 200); 
		Character->SetActorLocation(HouseLocation);
	}
}
