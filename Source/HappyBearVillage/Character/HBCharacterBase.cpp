// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HBCharacterBase.h"

#include "Net/UnrealNetwork.h"
#include "Weapon/HBWeaponBase.h"

// Sets default values
AHBCharacterBase::AHBCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	CurrentWeapon->SetupAttachment(GetMesh(), TEXT("RightHandWeaponSocket"));
	CurrentWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	CurrentWeapon->SetIsReplicated(true);
	CurrentWeapon->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshRef(
		TEXT("/Game/Assets/Fab/Baseball_Bat/baseball_bat/StaticMeshes/baseball_bat.baseball_bat"));
	if (WeaponMeshRef.Succeeded())
	{
		WeaponMesh = WeaponMeshRef.Object;
	}
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

void AHBCharacterBase::ServerRPCSetEquipped_Implementation(bool bEquipped)
{
	bWeaponEquipped = bEquipped;
	//OnRep_WeaponEquipped();

	SetWeaponMesh();
}

void AHBCharacterBase::EquipWeapon()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("EquipWeapon 호출"));
		
		bWeaponEquipped = true;
		OnRep_WeaponEquipped();
	}
	else
	{
		ServerRPCSetEquipped(true);
	}
}

void AHBCharacterBase::UnEquipWeapon()
{
	UE_LOG(LogTemp, Log, TEXT("UnEquipWeapon 호출"));
	if (CurrentWeapon->GetStaticMesh() != nullptr)
	{
		CurrentWeapon->SetStaticMesh(nullptr);
	}
}

void AHBCharacterBase::SetWeaponMesh()
{
	if (bWeaponEquipped == true)
	{
		if (WeaponMesh != nullptr)
		{
			if (CurrentWeapon->GetStaticMesh() == nullptr)
			{
				CurrentWeapon->SetStaticMesh(WeaponMesh);
			}
		}
	}
	
	else
	{
		if (CurrentWeapon->GetStaticMesh() != nullptr)
		{
			CurrentWeapon->SetStaticMesh(nullptr);
		}
	}
}

void AHBCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHBCharacterBase, bWeaponEquipped);
}

void AHBCharacterBase::OnRep_WeaponEquipped()
{
	GetWorldTimerManager().SetTimerForNextTick(this, &AHBCharacterBase::SetWeaponMesh);
	//SetWeaponMesh();
}

void AHBCharacterBase::AttackHitCheck()
{
}

