// Fill out your copyright notice in the Description page of Project Settings.


#include "HBPresent.h"

#include "GameFramework/ProjectileMovementComponent.h"


AHBPresent::AHBPresent()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
    
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMesh.Succeeded())
	{
		Mesh->SetStaticMesh(CubeMesh.Object);
	}

	Mesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5));
	Mesh->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 400.f;
	ProjectileMovement->MaxSpeed = 400.f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->Friction = 0.5f;

	bReplicates = true;
	AActor::SetReplicateMovement(true);
}

void AHBPresent::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && ProjectileMovement)
	{
		FVector LaunchDirection = (GetActorForwardVector() + FVector(0.f, 0.f, 0.5f)).GetSafeNormal();
		ProjectileMovement->Velocity = LaunchDirection * ProjectileMovement->InitialSpeed;
	}
}