// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/Job/HBJobPoliceComponent.h"

#include "Net/UnrealNetwork.h"

UHBJobPoliceComponent::UHBJobPoliceComponent()
{
	bIsRemainAction = false;
}

void UHBJobPoliceComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHBJobPoliceComponent, bIsRemainAction);
}

void UHBJobPoliceComponent::OnRep_IsRemainAction()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_IsRemainAction"));
}

void UHBJobPoliceComponent::NightPhaseBegin()
{
	Super::NightPhaseBegin();

	if (GetOwner()->HasAuthority())
	{
		bIsRemainAction = true;
		OnRep_IsRemainAction();
	}
}

void UHBJobPoliceComponent::Action()
{
	Super::Action();

	// 집 알아오기

	// 바인딩
	// 이미 집에있는사람 받아서 Seal 처리
	House->Enter.AddUObject();
	
	if (bIsActionActive == false)
	{
		return;
	}
	
	if (bIsRemainAction == false)
	{
		return;
	}

	if (GetCurrentPhase() != EGamePhase::Night)
	{
		return;
	}
}



// Seal
// 스킬 봉인, 무기 해제

// EXIT
// 해제

