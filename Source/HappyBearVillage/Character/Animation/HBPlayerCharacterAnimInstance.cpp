// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/HBPlayerCharacterAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UHBPlayerCharacterAnimInstance::UHBPlayerCharacterAnimInstance()
{
	// 이동 여부 최소값 설정
	MovingThreshold = 3.f;
}

void UHBPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 애님 인스턴스를 갖고있는 Character 갖고 오기
	Owner = Cast<ACharacter>(GetOwningActor());

	// Character 가 있으면
	if (Owner)
	{
		// Owner 의 CharacterMovementComponent 갖고오기
		Movement = Owner->GetCharacterMovement();
	}
}

void UHBPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if(Movement)
	{
		// 이동하는 속도
		Velocity = Movement->Velocity;

		// 이동 속력
		MoveSpeed = Velocity.Size2D();

		// Idle 여부(1(True), 0(False))
		bIsIdle = MoveSpeed < MovingThreshold;
	}
}
