#include "HBInvitation.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AHBInvitation::AHBInvitation()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 400.f;
	ProjectileMovement->MaxSpeed = 400.f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->Friction = 0.5f;

	bReplicates = true;
	AActor::SetReplicateMovement(true);
}

void AHBInvitation::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && ProjectileMovement)
	{
		FVector LaunchDirection = (GetActorForwardVector() + FVector(0.f, 0.f, 0.5f)).GetSafeNormal();
		ProjectileMovement->Velocity = LaunchDirection * ProjectileMovement->InitialSpeed;
	}
}
