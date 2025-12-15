// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/HBWeaponBase.h"

// Sets default values
AHBWeaponBase::AHBWeaponBase()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	RootComponent = Mesh;
	Mesh->SetRelativeScale3D(FVector(0.003f, 0.003f, 0.003f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshRef(
		TEXT("/Game/Assets/Fab/Baseball_Bat/baseball_bat/StaticMeshes/baseball_bat.baseball_bat"));
	if (WeaponMeshRef.Succeeded())
	{
		Mesh->SetStaticMesh(WeaponMeshRef.Object);
	}

	bReplicates = true;
	SetReplicateMovement(false);
}

// Called when the game starts or when spawned
void AHBWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AHBWeaponBase::IsEquipped() const
{
	// 장착 시 소켓으로 들어감
	// Owner 가 있으면 장착, 없으면 미장착된 상태
	return GetOwner() != nullptr;
}

