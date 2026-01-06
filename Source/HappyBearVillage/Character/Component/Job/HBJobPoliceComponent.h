// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Component/Job/HBJobBaseComponent.h"
#include "HBJobPoliceComponent.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBJobPoliceComponent : public UHBJobBaseComponent
{
	GENERATED_BODY()

public:
	UHBJobPoliceComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	virtual void NightPhaseBegin() override;
	
	virtual void Action() override;

	UFUNCTION()
	void OnRep_IsRemainAction();
	
protected:

private:
	// 밤에 능력 사용 가능 여부
	// true 면 사용 가능, false 면 불가
	UPROPERTY(ReplicatedUsing = OnRep_IsRemainAction)
	uint8 bIsRemainAction : 1;
};
