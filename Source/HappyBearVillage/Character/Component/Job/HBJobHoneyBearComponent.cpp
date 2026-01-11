// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/Job/HBJobHoneyBearComponent.h"

#include "Character/HBCharacterPlayer.h"
#include "Net/UnrealNetwork.h"
#include "Prop/HBHouse.h"

UHBJobHoneyBearComponent::UHBJobHoneyBearComponent()
{
	bIsRemainAction = false;
}

void UHBJobHoneyBearComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHBJobHoneyBearComponent, bIsRemainAction);
}

void UHBJobHoneyBearComponent::NightPhaseBegin()
{
	Super::NightPhaseBegin();
	if (GetOwner()->HasAuthority())
	{
		bIsRemainAction = true;
		OnRep_IsRemainAction();
	}
}

void UHBJobHoneyBearComponent::NightPhaseEnd()
{
	Super::NightPhaseEnd();
}

void UHBJobHoneyBearComponent::Action()
{
	Super::Action();

	if (!bIsActionActive)
	{
		return;
	}

	if (!bIsRemainAction)
	{
		return;
	}

	if (GetCurrentPhase() != EGamePhase::Night)
	{
		return;
	}

	AHBCharacterPlayer* OwnerCharacter = GetOwner<AHBCharacterPlayer>();
	if (!OwnerCharacter)
	{
		return;
	}

	if (!OwnerCharacter->IsLocallyControlled())
	{
		return;
	}

	if (OwnerCharacter->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("HoneyBear Action - HasAuthority"));
		StealStaminaFromTarget();
	}
	else
	{
		ServerRPCAction();
	}
}

void UHBJobHoneyBearComponent::OnRep_IsRemainAction()
{
	UE_LOG(LogTemp, Warning, TEXT("HONEYBEAR : OnRep_IsRemainAction"));
}

void UHBJobHoneyBearComponent::ServerRPCAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("HoneyBear Action - ServerRPCAction"));
	StealStaminaFromTarget();
}

void UHBJobHoneyBearComponent::StealStaminaFromTarget()
{
	// @PHYTODO : 임시로 집 안에 있을 때 사용하면 집주인 스테미너 훔치기
	UE_LOG(LogTemp, Warning, TEXT("HoneyBear StealStaminaFromTarget"));
	AHBMafiaGameState* HBGameState = GetWorld()->GetGameState<AHBMafiaGameState>();
	if (!HBGameState)
	{
		return;
	}
	
	AHBCharacterPlayer* OwnerCharacter = GetOwner<AHBCharacterPlayer>();
	if (!OwnerCharacter)
	{
		return;
	}

	AHBHouse* TargetHouse = DetectHouse();
	if (!TargetHouse)
	{
		return;
	}
	
	FLinearColor HouseOwnerColor = TargetHouse->GetHouseColor();
	AHBCharacterPlayer* HBTargetCharacter = HBGameState->GetPlayerByColor(HouseOwnerColor);

	if (HBTargetCharacter->GetCurrentStamina() > 0)
	{
		OwnerCharacter->SetCurrentStamina(OwnerCharacter->GetCurrentStamina() + 1);
		OwnerCharacter->OnRep_CurrentStamina();

		HBTargetCharacter->SetCurrentStamina(HBTargetCharacter->GetCurrentStamina() - 1);
		HBTargetCharacter->OnRep_CurrentStamina();

		bIsRemainAction = false;
		OnRep_IsRemainAction();
	}
}
