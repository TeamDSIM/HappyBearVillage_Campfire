// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractObject/HBHouseActor.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/HBCharacterPlayer.h"

AHBHouseActor::AHBHouseActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// ===== Root =====
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// ===== House Mesh =====
	HouseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HouseMesh"));
	HouseMesh->SetupAttachment(Root);
	HouseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ===== Enter Trigger =====
	EnterTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EnterTrigger"));
	EnterTrigger->SetupAttachment(Root);

	// 트리거 기본 설정
	EnterTrigger->SetBoxExtent(FVector(200.f));
	EnterTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EnterTrigger->SetCollisionObjectType(ECC_WorldStatic);
	EnterTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	EnterTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AHBHouseActor::BeginPlay()
{
	Super::BeginPlay();

	// ===== Overlap 이벤트 바인딩 =====
	if (EnterTrigger)
	{
		EnterTrigger->OnComponentBeginOverlap.AddDynamic(
			this, &AHBHouseActor::OnEnterOverlap
		);

		EnterTrigger->OnComponentEndOverlap.AddDynamic(
			this, &AHBHouseActor::OnExitOverlap
		);
	}
}

/* ================= Overlap ================= */

void AHBHouseActor::OnEnterOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// Player인지 확인
	AHBCharacterPlayer* Player = Cast<AHBCharacterPlayer>(OtherActor);
	if (!Player)
	{
		return;
	}

	if (OtherComp != Player->GetCapsuleComponent())
	{
		return;
	}

	// 서버에서만 처리
	if (!Player->HasAuthority())
	{
		return;
	}

	Player->EnterHouse();
}

void AHBHouseActor::OnExitOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	AHBCharacterPlayer* Player = Cast<AHBCharacterPlayer>(OtherActor);
	if (!Player)
	{
		return;
	}

	if (OtherComp != Player->GetCapsuleComponent())
	{
		return;
	}

	if (!Player->HasAuthority())
	{
		return;
	}

	Player->ExitHouse();
}




