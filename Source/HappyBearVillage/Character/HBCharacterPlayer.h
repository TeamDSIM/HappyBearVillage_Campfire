// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/HBCharacterBase.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Interface/HBCharacterHUDInterface.h"
#include "TimerManager.h"
#include "Component/Job/HBJobBaseComponent.h"
#include "HBCharacterPlayer.generated.h"

/**
 *
 */
class UInputMappingContext;
class UInputAction;
class UInputComponent;
class UHBUserHUDWidget;
class UHBCharacterMafiaAttackComponent;

/* ================= Night Flow ================= */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, int32 /* Stamina */)

UENUM(BlueprintType)
enum class EPlayerNightState : uint8
{
	InHouse UMETA(DisplayName = "In House"),
	Outside UMETA(DisplayName = "Outside")
};

/* ================================================= */


UCLASS()
class HAPPYBEARVILLAGE_API AHBCharacterPlayer : public AHBCharacterBase, public IHBCharacterHUDInterface
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

	//무기 탈/착용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> ToggleWeaponAction;

	// 직업 행동
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JobAction;

	/* ========== Night Flow : State ========== */
public:
	FOnStaminaChanged OnStaminaChanged;

protected:
	// 남은 외출 가능 횟수 (RepNotify로 HUD 갱신)
	UPROPERTY(ReplicatedUsing = OnRep_Stamina, EditAnywhere, BlueprintReadOnly, Category = "Night")
	int32 Stamina = 3;

	// 최대 외출 가능 횟수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Night")
	int32 MaxStamina = 3;

	// 집 안 / 밖 상태
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Night")
	EPlayerNightState NightState = EPlayerNightState::InHouse;

	// 이번 밤에 집 밖으로 나간 적 있는지
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Night")
	bool bExitedHouseThisNight = false;

	// 이전 밤(전날)에 집 밖으로 나갔는지 여부 (회복 규칙 처리용)
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Night")
	bool bExitedPreviousNight = false;

	// 캐시된 HUD 위젯 (로컬 클라이언트만 사용)
	UPROPERTY(Transient)
	TObjectPtr<UHBUserHUDWidget> CachedHUDWidget;

	// Stamina RepNotify
	UFUNCTION()
	void OnRep_Stamina();


	/* ========== Night Flow : Stamina Recovery (Timer) ========== */
protected:
	// 집 안에 있을 때 스태미나 회복
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Night|Stamina")
	int32 StaminaRecoverAmount = 1;

	// 스태미나가 회복되기까지 걸리는 시간 (초)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Night|Stamina")
	float StaminaRecoverInterval = 10.f;

	// 스태미나 회복 타이머
	FTimerHandle StaminaRecoverTimerHandle;


	void StartStaminaRecovery();
	void StopStaminaRecovery();

	UFUNCTION()
	void RecoverStaminaTick();

public:
	/* ========== Night Flow : House Interaction ========== */
	UFUNCTION(BlueprintCallable, Category = "Night")
	void EnterHouse();

	UFUNCTION(BlueprintCallable, Category = "Night")
	void ExitHouse();

	// 밤이 새로 시작될 때 상태 초기화 (GameState에서 호출)
	UFUNCTION()
	void ResetNightState();

	// 밤이 끝나고 낮으로 넘어갈 때 호출하여 회복 규칙을 처리
	void ProcessNightEnd();

	/* ========== Movement / Action ========== */
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

	UFUNCTION()
	void ToggleWeapon();

	UFUNCTION()
	void DoJobAction();


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

	UPROPERTY(VisibleAnywhere, Category = State)
	TObjectPtr<class UHBCharacterRagdollComponent> RagdollComponent;

	// 플레이어 색상 섹션 ===================================
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> HandMeshDynamicMaterial;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerColor)
	FLinearColor PlayerColor;

	UFUNCTION()
	void OnRep_PlayerColor();

	void SetRandomBaseColor();
	void ResetBaseColor();

	void ApplyNightColor(bool bIsNight);


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
	float ViewAngleThreshold = 0.5f; // degrees

	UPROPERTY();
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
	void ServerRPCNotifyMiss(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd,
	                         FVector_NetQuantizeNormal TraceDir, float HitCheckTime);

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

	// UI 섹션 ====================================================
protected:
	virtual void SetupHUDWidget(UHBUserHUDWidget* InHUDWidget) override;

	//무기 탈/착용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHBCharacterMafiaAttackComponent> MafiaAttackComp;

public:
	FORCEINLINE UHBCharacterMafiaAttackComponent* GetMafiaAttackComp() { return MafiaAttackComp; }


	// 직업 컴포넌트
public:
	FORCEINLINE UHBJobBaseComponent* GetJobComponent() const { return JobComponent; }

	UFUNCTION()
	void AssignJob(TSubclassOf<UHBJobBaseComponent> JobClass);

protected:
	UPROPERTY(Replicated)
	TObjectPtr<UHBJobBaseComponent> JobComponent;
};
