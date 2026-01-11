#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HBInvitation.generated.h"

UCLASS()
class HAPPYBEARVILLAGE_API AHBInvitation : public AActor
{
	GENERATED_BODY()
	
public:	
	AHBInvitation();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;
};
