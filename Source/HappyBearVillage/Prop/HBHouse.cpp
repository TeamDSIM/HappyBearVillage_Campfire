// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/HBHouse.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/HBCharacterPlayer.h"
#include "Components/ArrowComponent.h"
#include "InteractObject/HBHoneyPot.h"
#include "Net/UnrealNetwork.h"

AHBHouse::AHBHouse()
{
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	RootComponent = Arrow;

	HouseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HouseMesh"));
	HouseMesh->SetupAttachment(RootComponent);

	EnterTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EnterTrigger"));
	EnterTrigger->SetupAttachment(RootComponent);
	EnterTrigger->SetBoxExtent(FVector(1000.f));
	EnterTrigger->SetCollisionProfileName(TEXT("HouseEnterTrigger"));

	GateCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("GateCollision"));
	GateCollision->SetupAttachment(RootComponent);
	GateCollision->SetBoxExtent(FVector(1000.f));
	GateCollision->SetCollisionProfileName(TEXT("HouseGateCollision"));

	HoneyPot = CreateDefaultSubobject<UChildActorComponent>(TEXT("HoneyPotChild"));
	HoneyPot->SetupAttachment(RootComponent);
	HoneyPot->SetChildActorClass(AHBHoneyPot::StaticClass());
}

void AHBHouse::BeginPlay()
{
	Super::BeginPlay();

	if (EnterTrigger)
	{
		EnterTrigger->OnComponentBeginOverlap.AddDynamic(this, &AHBHouse::OnEnterOverlap);
		EnterTrigger->OnComponentEndOverlap.AddDynamic(this, &AHBHouse::OnExitOverlap);
	}
}

void AHBHouse::OnEnterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

	Player->EnterHouse();

	if (!OverlapCharacters.Contains(Player))
	{
		OverlapCharacters.Add(Player);
	}
	
	OnCharacterEnter.Broadcast(OtherActor);
}

void AHBHouse::OnExitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

	if (OverlapCharacters.Contains(Player))
	{
		OverlapCharacters.Remove(Player);
	}
	
	OnCharacterExit.Broadcast(OtherActor);
}
