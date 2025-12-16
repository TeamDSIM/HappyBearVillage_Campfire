// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HBLobbyWidget.generated.h"

/**
 * 
 */

class UMultiplayerSessionsSubsystem;

UCLASS()
class HAPPYBEARVILLAGE_API UHBLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:

	UPROPERTY()
	TObjectPtr< UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	void OnCreateSessionComplete(bool bWasSuccessful);
	void OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& Results, bool bWasSuccessful);
	
};
