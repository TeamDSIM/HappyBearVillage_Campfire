#include "HBInvitation.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

AHBInvitation::AHBInvitation()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMesh.Succeeded())
	{
		Mesh->SetStaticMesh(CubeMesh.Object);
	}

	Mesh->SetRelativeScale3D(FVector(0.1f, 0.2f, 0.01f));
	Mesh->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	
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
