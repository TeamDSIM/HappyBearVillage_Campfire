// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/Job/HBJobAssassinComponent.h"

#include "Character/HBCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameState/HBMafiaGameState.h"
#include "Net/UnrealNetwork.h"

UHBJobAssassinComponent::UHBJobAssassinComponent()
{
	bIsSprint = false;
	ActionDuration = 5.f;
}

void UHBJobAssassinComponent::Attack()
{
	Super::Attack();
}

void UHBJobAssassinComponent::Action()
{
	Super::Action();
	if (GetCurrentPhase() != EGamePhase::Night)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shaco Action : Not Night Phase"));
		return;
	}
	
	// 스킬 사용이 불가능하면 반환
	if (!bIsActionActive)
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

	// 서버
	if (OwnerCharacter->HasAuthority())
	{
		ApplyMoveSpeed(1000.f);
	}
	// 클라이언트
	else
	{
		// ServerRPC 로 돌림
		ServerRPCSetMoveSpeed(1000.f);
	}
}

void UHBJobAssassinComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHBJobAssassinComponent, MaxWalkSpeed);
}

void UHBJobAssassinComponent::ApplyMoveSpeed(float NewSpeed)
{
	// 서버에서만 실행
	AHBCharacterPlayer* OwnerCharacter = GetOwner<AHBCharacterPlayer>();
	if (!OwnerCharacter || !OwnerCharacter->HasAuthority())
	{
		return;
	}
	
	UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement();
	if (!MovementComponent)
	{
		return;
	}

	if (bIsSprint)
	{
		return;
	}

	// 서버의 변수 업데이트 -> OnRep 트리거
	MaxWalkSpeed = NewSpeed;

	// 서버의 실제 값 업데이트
	MovementComponent->MaxWalkSpeed = MaxWalkSpeed;

	GetWorld()->GetTimerManager().SetTimer(
		MoveSpeedTimerHandle,
		this,
		&UHBJobAssassinComponent::EndAction,
		ActionDuration,
		false
	);
}

void UHBJobAssassinComponent::EndAction()
{
	AHBCharacterPlayer* OwnerCharacter = GetOwner<AHBCharacterPlayer>();
	if (!OwnerCharacter || !OwnerCharacter->HasAuthority())
	{
		return;
	}
	
	bIsSprint = false;
	ApplyMoveSpeed(600.f);
}

void UHBJobAssassinComponent::OnRep_MaxWalkSpeed()
{
	// 클라이언트에서 호출됨
	AHBCharacterPlayer* OwnerCharacter = GetOwner<AHBCharacterPlayer>();
	if (OwnerCharacter)
	{
		UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement();
		if (MovementComponent)
		{
			MovementComponent->MaxWalkSpeed = MaxWalkSpeed;
		}
	}

}

void UHBJobAssassinComponent::ServerRPCSetMoveSpeed_Implementation(float NewSpeed)
{
	ApplyMoveSpeed(NewSpeed);
}
