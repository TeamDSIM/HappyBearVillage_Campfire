// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HBInGameHUDComponent.generated.h"


class UHBUserHUDWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAPPYBEARVILLAGE_API UHBInGameHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHBInGameHUDComponent();

public:
	void ActivateHUD(APlayerController* InPlayerController);
	void DeactivateHUD();

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> UserHUDWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UHBUserHUDWidget> InGameHUD;
	
		
};
