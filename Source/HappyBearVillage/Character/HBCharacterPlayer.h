// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/HBCharacterBase.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "HBCharacterPlayer.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
class UInputComponent;

UCLASS()
class HAPPYBEARVILLAGE_API AHBCharacterPlayer : public AHBCharacterBase
{
	GENERATED_BODY()

public:
	AHBCharacterPlayer();

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	// 입력 섹션
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MouseLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

public:
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Attack(const FInputActionValue& Value);

	UFUNCTION()
	void Interaction(const FInputActionValue& Value);

	UFUNCTION()
	void MouseLook(const FInputActionValue& Value);

	// 1인칭 카메라
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> FPSCameraComponent;

	// 1인칭 카메라 FOV
	UPROPERTY(EditAnywhere, Category = Camera)
	float FPSFieldOfView = 70.f;

	// 1인칭 카메라 View Scale
	UPROPERTY(EditAnywhere, Category = Camera)
	float FPSViewScale = 0.6f;

	// 1인칭 카메라에서만 보이는 메시
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> FPSMeshComponent;


	// 애님 섹션
protected:
	UPROPERTY(EditAnywhere, Category = Animation)
	TSubclassOf<UAnimInstance> AnimInstanceClass;
};
