// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractObject/HBHoneyPot.h"

#include "Character/HBCharacterPlayer.h"
#include "Character/Component/Job/HBJobHoneyBearComponent.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "Prop/HBHouse.h"

AHBHoneyPot::AHBHoneyPot()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube")
	);
	InteractMesh->SetStaticMesh(CubeMesh.Object);
	InteractMesh->SetCollisionProfileName(TEXT("InteractObject"));
}

void AHBHoneyPot::ExecuteInteraction(AActor* Interactor)
{
	Super::ExecuteInteraction(Interactor);

	if (!HasAuthority())
	{
		return;
	}
	
	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (!HBGameState)
	{
		return;
	}
	
	AHBHouse* HBHouse = Cast<AHBHouse>(GetAttachParentActor());
	if (!HBHouse)
	{
		UE_LOG(LogTemp, Error, TEXT("House Nullptr"));
		return;
	}
	
	FLinearColor Color = HBHouse->GetHouseColor();
	UE_LOG(LogTemp, Warning, TEXT("House Color : %s"), *Color.ToString());
	AHBCharacterPlayer* TargetCharacter = HBGameState->GetPlayerByColor(Color);
	if (!TargetCharacter)
	{
		return;
	}
	
	AHBCharacterPlayer* PlayerCharacter = Cast<AHBCharacterPlayer>(Interactor);
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Steal Stamina"));
		UHBJobHoneyBearComponent* HoneyBearComponent = Cast<UHBJobHoneyBearComponent>(PlayerCharacter->GetJobComponent());
		HoneyBearComponent->StealStamina(TargetCharacter);
	}
}

bool AHBHoneyPot::CanInteract(AActor* Interactor) const
{
	if (Super::CanInteract(Interactor) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("False"));
		return false;
	}

	// 꿀곰이면 상호작용 가능
	AHBCharacterPlayer* CharacterPlayer = Cast<AHBCharacterPlayer>(Interactor);
	if (CharacterPlayer)
	{
		if (CharacterPlayer->GetStat()->GetCharacterRole().Job == EJobType::HONEYBEAR)
		{
			UE_LOG(LogTemp, Warning, TEXT("True"));
			return true;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("False"));
	return false;
}
