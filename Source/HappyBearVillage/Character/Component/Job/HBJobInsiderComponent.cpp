// Fill out your copyright notice in the Description page of Project Settings.


#include "HBJobInsiderComponent.h"
#include "Character/HBCharacterPlayer.h"
#include "PlayerState/HBPlayerState.h"
#include "Engine/World.h"
#include "Prop/HBHouse.h"
#include "Prop/HBInvitation.h"

UHBJobInsiderComponent::UHBJobInsiderComponent()
{
	InvitationClass = AHBInvitation::StaticClass();
}

void UHBJobInsiderComponent::NightPhaseBegin()
{
	Super::NightPhaseBegin();
	GatherInvitees();
	ClearInvitations();
}

void UHBJobInsiderComponent::NightPhaseEnd()
{
	Super::NightPhaseEnd();
	Invitations.Empty();
}

void UHBJobInsiderComponent::Action()
{
	Super::Action();
	
	if (!bIsActionActive || GetCurrentPhase() != EGamePhase::Night)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Insider Action"));
	SpawnInvitation();
}

void UHBJobInsiderComponent::SpawnInvitation_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Insider SpawnInvitation"));
	AHBHouse* TargetHouse = DetectHouse();
	if (!TargetHouse) return;

	UE_LOG(LogTemp, Warning, TEXT("Insider DetectHouse Done"));
	
	FLinearColor HouseColor = TargetHouse->GetHouseColor();
	if (InviteeColors.Contains(HouseColor)) return;
	
	InviteeColors.Add(HouseColor);

	AHBCharacterPlayer* Insider = Cast<AHBCharacterPlayer>(GetOwner());
	FVector SpawnLocation = Insider->GetActorLocation() + Insider->GetActorForwardVector() * 50.f;
	FRotator SpawnRotation = Insider->GetActorRotation();
	
	AHBInvitation* Invitation = GetWorld()->SpawnActor<AHBInvitation>(InvitationClass, SpawnLocation, SpawnRotation);
	Invitations.Add(Invitation);
}

void UHBJobInsiderComponent::GatherInvitees()
{
	UE_LOG(LogTemp, Warning, TEXT("Insider GatherInvitees"));
	if (!GetOwner()->HasAuthority()) return;

	UE_LOG(LogTemp, Warning, TEXT("Insider GatherInvitees - 1"));

	for (APlayerState* PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		AHBPlayerState* HBPlayerState = Cast<AHBPlayerState>(PlayerState);
		AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(HBPlayerState->GetPawn());

		if (InviteeColors.Contains(Character->PlayerColor))
		{
			float RandomTheta = FMath::DegreesToRadians(FMath::FRandRange(0.f, 360.f));
			Character->SetActorLocation(GetOwner()->GetActorLocation() + FVector(FMath::Cos(RandomTheta), FMath::Sin(RandomTheta), 0.f) * 100);
			UE_LOG(LogTemp, Warning, TEXT("Insider GatherInvitees - 2"));
		}
	}
}

void UHBJobInsiderComponent::ClearInvitations()
{
	for (AHBInvitation* Invitation : Invitations)
	{
		Invitation->Destroy();
	}

	InviteeColors.Empty();
}
