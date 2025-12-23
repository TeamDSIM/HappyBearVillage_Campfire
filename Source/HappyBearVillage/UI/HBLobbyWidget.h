// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "HBLobbyWidget.generated.h"


/**
 * 
 */

class UMultiplayerSessionsSubsystem;
class UButton;


UCLASS()
class HAPPYBEARVILLAGE_API UHBLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetFriendInviteVisible(bool bVisible);

private:

	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UFUNCTION()
	void StartButtonClicked();

	UFUNCTION()
	void ExitButtonClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_FriendList;

	//bool bFriendInviteOpened=false;
};
