// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/Job/HBJobSleepwalkerComponent.h"
#include "Subsystem/HBVillageGenerationWorldSubsystem.h"
#include "Prop/HBVillage.h"


#include "Character/HBCharacterPlayer.h"
UHBJobSleepwalkerComponent::UHBJobSleepwalkerComponent()
{

}

void UHBJobSleepwalkerComponent::NightPhaseBegin()
{
	Super::NightPhaseBegin();

	//테스트용 위치
	//FVector TestLocation = FVector(0.0f, 0.0f, 1000.0f);

	UHBVillageGenerationWorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem< UHBVillageGenerationWorldSubsystem>();

	if (!WorldSubsystem)
	{
		UE_LOG(LogTemp, Log, TEXT("No UHBVillageGenerationWorldSubsystem"));
	}
	TArray<FVector> AvailableArray = WorldSubsystem->GetVillage()->GetAvailableBlockLocations();

	int32 Index = FMath::RandRange(0, AvailableArray.Num() - 1);
	FVector RandomAvailableLocation = AvailableArray[Index];

	UE_LOG(LogTemp, Log, TEXT("SleepWalkerTry"));
	AHBCharacterPlayer* HBCharacterPlayer = GetOwner<AHBCharacterPlayer>();
	if (HBCharacterPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("SleepWalkerTry TestLocate"));
		HBCharacterPlayer->SetActorLocation(RandomAvailableLocation);
		HBCharacterPlayer->SetCurrentStamina(HBCharacterPlayer->GetMaxStamina());
	}


}
