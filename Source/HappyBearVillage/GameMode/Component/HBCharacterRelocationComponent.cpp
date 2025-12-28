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
		HouseLocation = FVector(HouseLocation.X, HouseLocation.Y, 200); 
		Character->SetActorLocation(HouseLocation);
	}
}

void UHBCharacterRelocationComponent::RelocateCharacterToCouncil(AHBMafiaGameState* InGameState)
{
	if (!InGameState) return;
	if (!GetOwner() || !GetWorld()) return;

	//  서버에서만 실행
	if (!GetOwner()->HasAuthority())
		return;

	//회의장 위치로 이동
	const FVector CouncilLocation = FVector(200000.0f, 0.0f, 92.0f);

	for (APlayerState* PlayerState : InGameState->PlayerArray)
	{
		if (!PlayerState) continue;

		AController* Controller = PlayerState->GetPlayerController();
		if (!Controller) continue;

		AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(Controller->GetPawn());
		if (!Character) continue;

		// 순간이동
		Character->SetActorLocation(CouncilLocation);
	}
}
