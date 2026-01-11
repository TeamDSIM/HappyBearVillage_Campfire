// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Component/Job/HBJobBaseComponent.h"
#include "HBJobHoneyBearComponent.generated.h"

/**
 * 
 */


UCLASS()
class HAPPYBEARVILLAGE_API UHBJobHoneyBearComponent : public UHBJobBaseComponent
{
	GENERATED_BODY()

public:
	UHBJobHoneyBearComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	virtual void NightPhaseBegin() override;
	virtual void NightPhaseEnd() override;
	
	virtual void Action() override;

	UFUNCTION()
	void OnRep_IsRemainAction();

protected:
	UFUNCTION(Server, Reliable)
	void ServerRPCAction();

	UFUNCTION()
	void StealStaminaFromTarget();

private:
	UPROPERTY(ReplicatedUsing = OnRep_IsRemainAction)
	uint8 bIsRemainAction : 1;
};
