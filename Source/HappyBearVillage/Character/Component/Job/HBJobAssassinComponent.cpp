// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/Job/HBJobAssassinComponent.h"

#include "Character/HBCharacterPlayer.h"
#include "Character/Stat/HBPlayerStatComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameState/HBMafiaGameState.h"
#include "Net/UnrealNetwork.h"

UHBJobAssassinComponent::UHBJobAssassinComponent()
{
	bIsSprint = false;
	ActionDuration = 5.f;
}

void UHBJobAssassinComponent::Attack(AActor* HitActor)
{
	Super::Attack(HitActor);

	if (GetCurrentPhase() != EGamePhase::Night)
	{
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Assassin Attack"));
	
	// 피격 대상이 없으면 반환
	if (!HitActor)
	{
		return;
	}

	AActor* OwnerActor = GetOwner<AActor>();
	if (!OwnerActor)
	{
		return;
	}

	AHBCharacterPlayer* HitCharacter = Cast<AHBCharacterPlayer>(HitActor);
	if (!HitCharacter)
	{
		return;
	}

	// 체력이 0일 시 한번 더 백어택 판정 들어가는 것을 방지
	if (HitCharacter->GetStat()->GetHealth() <= 0)
	{
		return;
	}

	// 공격자의 앞방향 (Pawn 이 있으면 ControlRotation 으로 받음)
	FVector OwnerForward = OwnerActor->GetActorForwardVector();
	APawn* OwnerPawn = Cast<APawn>(OwnerActor);
	if (OwnerPawn)
	{
		OwnerForward = OwnerPawn->GetControlRotation().Vector();
	}
	OwnerForward.Z = 0.f;
	OwnerForward.Normalize();

	// 피격자의 앞방향 (Pawn 이 있으면 ControlRotation 으로 받음)
	FVector TargetForward = HitActor->GetActorForwardVector();
	APawn* TargetPawn = Cast<APawn>(HitActor);
	if (TargetPawn)
	{
		TargetForward = TargetPawn->GetControlRotation().Vector();
	}
	TargetForward.Z = 0.f;
	TargetForward.Normalize();

	// 두 캐릭터의 앞방향 내적 (얼마나 같은 곳을 보고 있는가)
	float OrientationDot = FVector::DotProduct(OwnerForward, TargetForward);

	// 공격자가 피격자의 뒤에 위치하는지 확인
	FVector OwnerLoc = OwnerActor->GetActorLocation();
	FVector TargetLoc = HitActor->GetActorLocation();
	OwnerLoc.Z = 0.f;
	TargetLoc.Z = 0.f;
	FVector TargetToOwner = (OwnerLoc - TargetLoc).GetSafeNormal();
	
	float PositionDot = FVector::DotProduct(TargetForward, TargetToOwner);

	UE_LOG(LogTemp, Log, TEXT("OrientationDot: %f (1에 가까울수록 같은 방향)"), OrientationDot);
	UE_LOG(LogTemp, Log, TEXT("PositionDot: %f (-1에 가까울수록 피격자 뒤에 위치)"), PositionDot);

	// 최종 판정: 
	// 두 캐릭터가 바라보는 방향이 비슷하고 (OrientationDot > 0.85) (수치가 작아질수록 후방 판정 범위가 넓어짐)
	// 공격자가 피격자의 뒤쪽 공간에 위치함 (PositionDot < -0.5)
	if (OrientationDot > 0.85f && PositionDot < -0.5f)
	{
		UE_LOG(LogTemp, Log, TEXT("BackAttack"));
		FDamageEvent DamageEvent;
		HitActor->TakeDamage(10, DamageEvent, GetOwner<AHBCharacterPlayer>()->GetController(), GetOwner());
	}
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
	ApplyMoveSpeed(800.f);
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
