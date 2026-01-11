// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HBJobBaseComponent.h"
#include "HBJobInsiderComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HAPPYBEARVILLAGE_API UHBJobInsiderComponent : public UHBJobBaseComponent
{
	GENERATED_BODY()

public:
	UHBJobInsiderComponent();

	virtual void NightPhaseBegin() override;
	virtual void NightPhaseEnd() override;
	virtual void Action() override;

protected:
	UFUNCTION(Server, Reliable)
	void SpawnInvitation();
	void GatherInvitees();
	void ClearInvitations();
	
protected:
	UPROPERTY()
	TArray<class AHBInvitation*> Invitations;

	UPROPERTY()
	TSubclassOf<class AHBInvitation> InvitationClass;
	
	UPROPERTY()
	TArray<FLinearColor> InviteeColors;
};
