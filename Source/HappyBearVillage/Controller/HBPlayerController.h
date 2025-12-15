// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HBPlayerController.generated.h"

class UHBLobbyWidget;

UCLASS()
class HAPPYBEARVILLAGE_API AHBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AHBPlayerController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UHBLobbyWidget> HBLobbyWidgetClass;

	UPROPERTY()
	TObjectPtr<class UHBLobbyWidget> HBLobbyWidget;

};
