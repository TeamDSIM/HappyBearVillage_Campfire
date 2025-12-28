// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HBCharacterMafiaAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAPPYBEARVILLAGE_API UHBCharacterMafiaAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UHBCharacterMafiaAttackComponent();
    void ToggleWeapon();

private:
    UFUNCTION(Server, Reliable)
    void Server_ToggleWeapon();

    bool CanToggleWeapon() const;
		
};
