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

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FORCEINLINE bool GetCanActiveEquip() const { return bIsCanActiveEquip; }
	FORCEINLINE void SetCanActiveEquip(bool bInCanActiveEquip) { bIsCanActiveEquip = bInCanActiveEquip; }

private:
    UFUNCTION(Server, Reliable)
    void Server_ToggleWeapon();

    bool CanToggleWeapon() const;

private:
	// 무기 장착 버튼 가능 여부
	UPROPERTY(Replicated)
	uint8 bIsCanActiveEquip : 1;
		
};
