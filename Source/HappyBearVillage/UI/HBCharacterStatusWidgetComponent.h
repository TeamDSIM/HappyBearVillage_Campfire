// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HBCharacterStatusWidgetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HAPPYBEARVILLAGE_API UHBCharacterStatusWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHBCharacterStatusWidgetComponent();

	virtual void BeginPlay() override;
	void BindWidgetEvents(class AHBCharacterPlayer* InCharacter);
	void ActivateHUD(APlayerController* InPlayerController);
	void DeactivateHUD();

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UHBNightStaminaWidget> NightStaminaWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UHBNightStaminaWidget> NightStaminaWidget;
	
};
