// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HBPlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UHBPlayerCharacterAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	// 불루프린트용 변수
protected:
	// 참조 변수.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// 이동하는 속도(무브먼트 컴포넌트에서 읽기).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	FVector Velocity;

	// 이동 속력
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float MoveSpeed;

	// 이동 방향
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float Direction = 0.f;

	// 이동 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsIdle : 1;

	// 이동 여부를 판단할 때 사용할 최소값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float MovingThreshold;

	// 떨어지는지 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	// 점프 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	// 점프 여부를 판단할 때 사용할 최소값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshold;
};
