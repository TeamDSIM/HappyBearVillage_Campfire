// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HBCharacterBase.h"

#include "Net/UnrealNetwork.h"
#include "Weapon/HBWeaponBase.h"

// Sets default values
AHBCharacterBase::AHBCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AHBCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void AHBCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHBCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AHBCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AHBCharacterBase::EquipWeapon(TSubclassOf<AHBWeaponBase> WeaponClass)
{
	UE_LOG(LogTemp, Log, TEXT("EquipWeapon 호출"));
	if (!WeaponClass)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = this;

	AHBWeaponBase* NewWeapon = GetWorld()->SpawnActor<AHBWeaponBase>(
		WeaponClass,
		FTransform::Identity,
		Params
	);

	if (NewWeapon)
	{
		UE_LOG(LogTemp, Log, TEXT("NewWeapon = %s"), *NewWeapon->GetName());
		UE_LOG(LogTemp, Log, TEXT("EquipWeapon NetMode: %d, HasAuthority: %d"), (int32)GetNetMode(), HasAuthority());
		UE_LOG(LogTemp, Warning, TEXT("World: %s"),	*GetWorld()->GetName());
		CurrentWeapon = NewWeapon;
	}
}

void AHBCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHBCharacterBase, CurrentWeapon);
}

void AHBCharacterBase::OnRep_CurrentWeapon()
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_CurrentWeapon 호출"));

	if (CurrentWeapon != nullptr)
	{
		return;
	}

	CurrentWeapon->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("RightHandWeaponSocket")
	);
}
