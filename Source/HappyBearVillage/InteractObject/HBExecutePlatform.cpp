// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractObject/HBExecutePlatform.h"

#include "Subsystem/HBGameVoteSubsystem.h"

// Sets default values
AHBExecutePlatform::AHBExecutePlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(Root);

	TargetPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("TargetPosition"));
	TargetPosition->SetupAttachment(PlatformMesh);
}

// Called when the game starts or when spawned
void AHBExecutePlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UHBGameVoteSubsystem* VoteSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UHBGameVoteSubsystem>();
		if (VoteSubsystem)
		{
			if (VoteSubsystem->GetExecutePlatform() == nullptr)
			{
				VoteSubsystem->SetExecutePlatform(this);
			}
		}
	}
}


