// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Interface/HBAttackAnimationInterface.h"
#include "Interface/HBCharacterWidgetInterface.h"
#include "HBCharacterBase.generated.h"

class UHBPlayerStatComponent;
class AHBWeaponBase;

UCLASS()
class HAPPYBEARVILLAGE_API AHBCharacterBase : public ACharacter, public IHBAttackAnimationInterface,
                                              public IHBCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHBCharacterBase();

	// 나중에 델리게이트 함수 바인딩용
	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// ACharacter 에 있는 것
	// Mesh, CharacterMovement, CapsuleComponent, Arrow

	// @Todo:
	// 추가해야할 것
	// 스탯, 애니메이션, 공격, 기절, 무기 장착

	// UI 섹션 ==============================================================
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<UWidgetComponent> TotalDamageWidget;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<UWidgetComponent> PlayerRoleWidget;

	virtual void SetUpCharacterWidget(class UHBUserWidget* InUserWidget) override;

	// 스탯 관련 섹션 =========================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHBPlayerStatComponent> Stat;

public:
	FORCEINLINE UHBPlayerStatComponent* GetStat() const { return Stat; }

	// 무기 장착 관련 섹션 =====================================================
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> WeaponMesh;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponEquipped)
	uint8 bWeaponEquipped : 1;

	UFUNCTION(Server, Reliable)
	void ServerRPCSetEquipped(bool bEquipped);

	//무기 장착 여부 함수
public:
	bool IsWeaponEquipped();

public:
	FORCEINLINE UStaticMeshComponent* GetCurrentWeapon() { return CurrentWeapon; }

	void EquipWeapon();
	void UnEquipWeapon();

	virtual void SetWeaponMesh();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_WeaponEquipped();

	// 인터페이스 구현
protected:
	virtual void AttackHitCheck() override;

	// 공격 판정 처리 구현
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;
};
