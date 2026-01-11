// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Component/Job/HBJobBaseComponent.h"
#include "HBJobSantaComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HAPPYBEARVILLAGE_API UHBJobSantaComponent : public UHBJobBaseComponent
{
	GENERATED_BODY()

public:
	UHBJobSantaComponent();

	virtual void NightPhaseBegin() override;
	virtual void NightPhaseEnd() override;
	virtual void Action() override;

protected:
	UFUNCTION(Server, Reliable)
	void SpawnPresent();
	void ApplyLastNightPresentAbility();

protected:
	UPROPERTY()
	TObjectPtr<class AHBPresent> LastNightPresent;

	UPROPERTY()
	TObjectPtr<class AHBPresent> TonightPresent;
	
	UPROPERTY()
	TSubclassOf<class AHBPresent> PresentClass;
	
	UPROPERTY()
	FLinearColor LastNightTargetColor;
	
	UPROPERTY()
	FLinearColor TonightTargetColor;

	UPROPERTY()
	uint8 bUseAbilityTonight : 1;
	
};
