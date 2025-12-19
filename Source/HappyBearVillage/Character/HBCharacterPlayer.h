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
	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;


	// 입력 섹션 ===========================================
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

	// @PHYTODO : 직업 분배 임시 확인용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> StartAction;
	
public:
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void Interaction();

	UFUNCTION()
	void MouseLook(const FInputActionValue& Value);

	// @PHYTODO : 직업 분배 임시 확인용
	
	UFUNCTION()
	void Start();

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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> FPSCurrentWeapon;

	// 플레이어 색상 섹션 ===================================
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerColor)
	FLinearColor PlayerColor;

	UFUNCTION()
	void OnRep_PlayerColor();

	void SetRandomBaseColor();


	// 애님 섹션 ===========================================
protected:
	UPROPERTY(EditAnywhere, Category = Animation)
	TSubclassOf<UAnimInstance> AnimInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage> AttackMontage;

	// 상호작용 섹션 ===========================================
protected:
	FTimerHandle InteracticonTraceTimerHandle;

	// 플레이어의 시선 비교용
	FRotator LastControlRotation;

	// 상호작용 트레이스할 거리
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	float InteractionDistance = 200.f;

	// trace 할 시간 간격 값
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	float InteractionTraceInterval = 0.1f; // 10Hz

	// 시선 변경 시 trace 할 각도 값
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	float ViewAngleThreshold = 0.5f;	// degrees

	UPROPERTY()
	TObjectPtr<AActor> InteractionTarget;

	UFUNCTION()
	void InteractionTraceTick();

	virtual void SetWeaponMesh() override;

	// 공격 섹션 ===========================================
	UPROPERTY()
	uint8 bCanAttack : 1;

	virtual void AttackHitCheck() override;
	void AttackHitConfirm(AActor* HitActor);
	void DrawDebugAttackRange(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCAttack(float AttackStartTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifyHit(const FHitResult& HitResult, float HitCheckTime);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifyMiss(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime);

	UFUNCTION(Client, Unreliable)
	void ClientRPCPlayAnimation(AHBCharacterPlayer* CharacterToPlay);

	float AttackTime = 2.2667f;
	float LastAttackStartTime = 0.f;
	float AttackTimeDifference = 0.f;
	float AcceptCheckDistance = 300.f;
	float AcceptMinCheckTime = 0.15f;

	void PlayAttackAnimation();

	
	// @PHYTODO : 직업 분배 임시 확인용
	UFUNCTION(Server, Reliable)
	void ServerRPCStart();
	
};
