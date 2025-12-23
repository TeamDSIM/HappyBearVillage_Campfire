// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HBSteamFriendEntryWidget.generated.h"

/**
 * 
 */

class UTextBlock;
class UButton;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInviteClicked, const FString& /*NetIdStr*/);

UCLASS()
class HAPPYBEARVILLAGE_API UHBSteamFriendEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const FString& InName, const FString& InNetIdStr, bool bOnline);

	FOnInviteClicked OnInviteClicked;
		
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> FriendNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> InviteButton;

private:
	FString NetIdStr;

	UFUNCTION()
	void HandleInviteClicked();

};
