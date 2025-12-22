// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HBGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:

	UHBGameInstance();

	virtual void Init() override;
	
private:
	UFUNCTION()
	void OnPostLoadMap(UWorld* LoadedWorld);

public:
	UPROPERTY()
	bool bNewMapLoad = false;

	
};
