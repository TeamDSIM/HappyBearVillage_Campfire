// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Component/Job/HBJobBaseComponent.h"
#include "HBJobAssassinComponent.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBJobAssassinComponent : public UHBJobBaseComponent
{
	GENERATED_BODY()

public:
	UHBJobAssassinComponent();

public:
	virtual void Attack() override;
	
	virtual void Action() override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	UFUNCTION(Server, Reliable)
	void ServerRPCSetMoveSpeed(float NewSpeed);

	void ApplyMoveSpeed(float NewSpeed);

	void EndAction();

	UFUNCTION()
	void OnRep_MaxWalkSpeed();

private:
	FTimerHandle MoveSpeedTimerHandle;

	// 현재 스킬 적용 여부
	float ActionDuration = 5.f;
	uint8 bIsSprint : 1;

	UPROPERTY(ReplicatedUsing = OnRep_MaxWalkSpeed)
	float MaxWalkSpeed = 0.f;

	
	
};
