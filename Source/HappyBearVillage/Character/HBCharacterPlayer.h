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
#include "Stat/HBCharacterRole.h"
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
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentStaminaChanged, int32 /* Stamina */)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerHealthChanged, int32 /* Health */)

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerJobChanged, EJobType /* Job */)

/* ================================================= */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPoliceEffectChanged, bool /* bHasPoliceEffect */)

UCLASS()
class HAPPYBEARVILLAGE_API AHBCharacterPlayer : public AHBCharacterBase, public IHBCharacterHUDInterface
{
	GENERATED_BODY()

public:
	AHBCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

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

	// ===== Dance Input =====
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> DanceAction;

	/* ========== Night Flow : State ========== */
public:
	FOnCurrentStaminaChanged OnCurrentStaminaChanged;
	FOnPoliceEffectChanged OnPoliceEffectChanged;
	FOnPlayerHealthChanged OnPlayerHealthChanged;
	FOnPlayerJobChanged OnPlayerJobChanged;

protected:
	// 현재 밤에 적용되는 스태미너
	UPROPERTY(ReplicatedUsing = OnRep_CurrentStamina, EditAnywhere, BlueprintReadOnly, Category = "Night")
	int32 CurrentStamina = 2;

	// 밤에 넘어왔을 때 기준 스태미너 (Current 와 다르면 이번엔 나갔다는 기준)
	UPROPERTY(ReplicatedUsing = OnRep_NightStamina, EditAnywhere, BlueprintReadOnly, Category = "Night")
	int32 NightStamina = 2;

	// 최대 외출 가능 횟수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Night")
	int32 MaxStamina = 2;

	// 아침 초기화 시 콜리젼 변경 여부
	UPROPERTY(ReplicatedUsing = OnRep_InitNightCollision, VisibleAnywhere, BlueprintReadOnly, Category = "Night")
	bool bInitNightCollision = false;
	
	// 이번 밤에 집 밖으로 나간 적 있는지
	UPROPERTY(ReplicatedUsing = OnRep_ExitedHouseThisNight, VisibleAnywhere, BlueprintReadOnly, Category = "Night")
	bool bExitedHouseThisNight = false;

	// 이전 밤(전날)에 집 밖으로 나갔는지 여부 (회복 규칙 처리용)
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Night")
	bool bExitedPreviousNight = true;

	// 캐시된 HUD 위젯 (로컬 클라이언트만 사용)
	UPROPERTY(Transient)
	TObjectPtr<UHBUserHUDWidget> CachedHUDWidget;

public:
	// Stamina RepNotify
	UFUNCTION()
	void OnRep_CurrentStamina();

	UFUNCTION()
	void OnRep_NightStamina();
	
	UFUNCTION()
	void OnRep_InitNightCollision();

	UFUNCTION()
	void OnRep_ExitedHouseThisNight();


	/* ========== Night Flow : Stamina Recovery (Timer) ========== */
public:
	void StaminaRecovery();

public:
	/* ========== Night Flow : House Interaction ========== */
	UFUNCTION(BlueprintCallable, Category = "Night")
	void EnterHouse();

	UFUNCTION(BlueprintCallable, Category = "Night")
	void ExitHouse();

	// 밤이 새로 시작될 때 상태 초기화 (GameState에서 호출)
	UFUNCTION()
	void ResetNightState();

	// 밤이 끝나고 낮으로 넘어갈 때
	void ProcessNightEnd();

	FORCEINLINE int32 GetCurrentStamina() const { return CurrentStamina; }
	FORCEINLINE void SetCurrentStamina(int32 NewStamina)
	{
		if (HasAuthority())
		{
			CurrentStamina = NewStamina;
			OnRep_CurrentStamina();

			UE_LOG(LogTemp, Log, TEXT("SetCurrentStamina called %d"), CurrentStamina);
		}
	}

	FORCEINLINE int32 GetMaxStamina() const { return MaxStamina; }

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

	// ===== Dance Functions =====
	UFUNCTION()
	void Dance();

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
	UPROPERTY(VisibleAnywhere, Category = "Material")
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Material")
	TObjectPtr<UMaterialInstanceDynamic> HandMeshDynamicMaterial;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerColor)
	FLinearColor PlayerColor;

	UPROPERTY(ReplicatedUsing = OnRep_RenderColor)
	FLinearColor RenderColor;

	UFUNCTION()
	void OnRep_PlayerColor();

	UFUNCTION()
	void OnRep_RenderColor();

	void SetFPSMeshColor();
	void ResetBaseColor();

	void ChangeCharacterMaterialColor(FLinearColor NewColor);

	// 피격 시 색상 변경
	void ApplyHitFlash();
	void UpdateHitFlash();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCApplyHitFlash(float Duration);

private:
	FTimerHandle HitFlashTimerHandle;
	
	float HitFlashDuration = 0.15f;
	float HitFlashElapsed = 0.f;


	// 애님 섹션 ===========================================
protected:
	UPROPERTY(EditAnywhere, Category = Animation)
	TSubclassOf<UAnimInstance> AnimInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage> AttackMontage;

	// ===== Dance Animation =====
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Dance")
	TArray<TObjectPtr<UAnimMontage>> DanceMontages;

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

	UFUNCTION(Client, Unreliable)
	void ClientRPCPlayHitVignette(float Intensity);

	float AttackTime = 2.2667f;
	float AttackPlayRate = 1.5f;
	float LastAttackStartTime = 0.f;
	float AttackTimeDifference = 0.f;
	float AcceptCheckDistance = 300.f;
	float AcceptMinCheckTime = 0.15f;

	void PlayAttackAnimation();

	// Dance 섹션 ===========================================

	// 마지막 춤 인덱스 (연속 중복 방지용, 선택)
	int32 LastDanceIndex = -1;

	UFUNCTION(Server, Reliable)
	void ServerRPCDance(int32 MontageIndex, float StartTime);

	UFUNCTION(Client, Unreliable)
	void ClientRPCPlayDance(AHBCharacterPlayer* CharacterToPlay, int32 MontageIndex, float StartTime);

	void PlayDanceAnimation(int32 MontageIndex);

	// ===== Weapon Control =====
	void SetWeaponVisibleForDance(bool bVisible);

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

public:
	UFUNCTION(Client, Reliable)
	void ClientRPCSetPoliceNotice(bool bIsSealed);
};
