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
}

void UHBJobInsiderComponent::NightPhaseEnd()
{
	Super::NightPhaseEnd();
	DestroyInvitations();
}

void UHBJobInsiderComponent::Action()
{
	Super::Action();
	
	if (!bIsActionActive || GetCurrentPhase() != EGamePhase::Night)
	{
		return;
	}
	
	SpawnInvitation();
}

void UHBJobInsiderComponent::SpawnInvitation_Implementation()
{
	AHBHouse* TargetHouse = DetectHouse();
	if (!TargetHouse) return;
	
	UE_LOG(LogTemp, Log, TEXT("House Detected"));
	
	FLinearColor HouseColor = TargetHouse->GetHouseColor();
	if (InviteeColors.Contains(HouseColor)) return;

	UE_LOG(LogTemp, Log, TEXT("Register Color"));
	
	InviteeColors.Add(HouseColor);

	AHBCharacterPlayer* Insider = Cast<AHBCharacterPlayer>(GetOwner());
	FVector SpawnLocation = Insider->GetActorLocation() + Insider->GetActorForwardVector() * 50.f;
	FRotator SpawnRotation = Insider->GetActorRotation();

	UE_LOG(LogTemp, Log, TEXT("Spawn Invitation"));
	AHBInvitation* Invitation = GetWorld()->SpawnActor<AHBInvitation>(InvitationClass, SpawnLocation, SpawnRotation);
	Invitations.Add(Invitation);
}

void UHBJobInsiderComponent::GatherInvitees()
{
	if (!GetOwner()->HasAuthority()) return;

	for (APlayerState* PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		AHBPlayerState* HBPlayerState = Cast<AHBPlayerState>(PlayerState);
		AHBCharacterPlayer* Character = Cast<AHBCharacterPlayer>(HBPlayerState->GetPawn());

		if (InviteeColors.Contains(Character->PlayerColor))
		{
			float RandomTheta = FMath::DegreesToRadians(FMath::FRandRange(0.f, 360.f));
			Character->SetActorLocation(GetOwner()->GetActorLocation() + FVector(FMath::Cos(RandomTheta), FMath::Sin(RandomTheta), 0.f) * 50);
			UE_LOG(LogTemp, Log, TEXT("Gather Invitee"));
		}
	}

	InviteeColors.Empty();
}

void UHBJobInsiderComponent::DestroyInvitations()
{
	for (AHBInvitation* Invitation : Invitations)
	{
		Invitation->Destroy();
	}

	Invitations.Empty();
}
