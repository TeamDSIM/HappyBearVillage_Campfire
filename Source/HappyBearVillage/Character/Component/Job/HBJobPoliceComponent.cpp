// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/Job/HBJobPoliceComponent.h"

#include "Character/HBCharacterPlayer.h"
#include "Character/Component/HBCharacterMafiaAttackComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/TriggerBox.h"
#include "Net/UnrealNetwork.h"
#include "Prop/HBHouse.h"

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

void UHBJobPoliceComponent::ServerRPCAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Police Action - ServerRPCAction"));
	BindSealEvent();
}

void UHBJobPoliceComponent::BindSealEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Police BindSealEvent"));
	// 집 알아오기
	AHBCharacterPlayer* CharacterPlayer = GetOwner<AHBCharacterPlayer>();
	if (!CharacterPlayer)
	{
		return;
	}
	
	TArray<AActor*> OverlappingActors;
	AHBHouse* HBHouse = nullptr;
	CharacterPlayer->GetCapsuleComponent()->GetOverlappingActors(OverlappingActors, AHBHouse::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		AHBHouse* House = Cast<AHBHouse>(Actor);
		if (House)
		{
			HBHouse = House;
			break;
		}
	}
		
	// 바인딩
	// 이미 집에있는사람 받아서 Seal 처리
	if (HBHouse)
	{
		// 이번 밤 재사용 막기
		bIsRemainAction = false;
		OnRep_IsRemainAction();
		
		for (AHBCharacterPlayer* InsidePlayer : HBHouse->GetOverlapCharacters())
		{
			SealPlayer(InsidePlayer);
		}
		
		HBHouse->OnCharacterEnter.AddUObject(this, &UHBJobPoliceComponent::SealPlayer);
		HBHouse->OnCharacterExit.AddUObject(this, &UHBJobPoliceComponent::UnsealPlayer);
	}
}

void UHBJobPoliceComponent::SealPlayer(AActor* InActor)
{
	if (InActor->HasAuthority())
	{		
		AHBCharacterPlayer* Player = Cast<AHBCharacterPlayer>(InActor);
		if (!Player)
		{
			return;
		}

		// 무기 해제
		Player->UnEquipWeapon();

		// 무기 장착 불가
		UHBCharacterMafiaAttackComponent* CharacterMafiaAttackComponent = Player->GetMafiaAttackComp();
		if (CharacterMafiaAttackComponent)
		{
			CharacterMafiaAttackComponent->SetCanActiveEquip(false);
		}

		// 능력 사용 불가
		Player->GetJobComponent()->SetIsActionActive(false);
		Player->GetJobComponent()->OnRep_IsActionActive();

		UE_LOG(LogTemp, Warning, TEXT("SealPlayer"));
		
		Player->ClientRPCSetPoliceNotice(true);
	}
}

void UHBJobPoliceComponent::UnsealPlayer(AActor* InActor)
{
	if (InActor->HasAuthority())
	{
		AHBCharacterPlayer* Player = Cast<AHBCharacterPlayer>(InActor);
		if (!Player)
		{
			return;
		}

		// 무기 장착 가능
		UHBCharacterMafiaAttackComponent* CharacterMafiaAttackComponent = Player->GetMafiaAttackComp();
		if (CharacterMafiaAttackComponent)
		{
			CharacterMafiaAttackComponent->SetCanActiveEquip(true);
		}

		// 능력 사용 가능
		Player->GetJobComponent()->SetIsActionActive(true);
		Player->GetJobComponent()->OnRep_IsActionActive();
		
		UE_LOG(LogTemp, Warning, TEXT("UnSeal Player"));
		
		Player->ClientRPCSetPoliceNotice(false);
	}
}

void UHBJobPoliceComponent::NightPhaseBegin()
{
	Super::NightPhaseBegin();

	if (GetOwner()->HasAuthority())
	{
		bIsRemainAction = true;
		OnRep_IsRemainAction();
		
		UE_LOG(LogTemp, Warning, TEXT("Police NightPhaseBegin"));
	}
}

void UHBJobPoliceComponent::Action()
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
		UE_LOG(LogTemp, Warning, TEXT("Police Action - HasAuthority"));
		BindSealEvent();
	}
	else
	{
		ServerRPCAction();
	}
}
