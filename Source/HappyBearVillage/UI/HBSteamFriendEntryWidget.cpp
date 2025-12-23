// Fill out your copyright notice in the Description page of Project Settings.


#include "HBSteamFriendEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UHBSteamFriendEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InviteButton)
	{
		InviteButton->OnClicked.AddDynamic(this, &ThisClass::HandleInviteClicked);
	}
}

void UHBSteamFriendEntryWidget::Init(const FString& InName, const FString& InNetIdStr, bool bOnline)
{
	NetIdStr = InNetIdStr;

	if (FriendNameText)
	{
		const FString Suffix = bOnline ? TEXT(" (Online)") : TEXT(" (Offline)");
		FriendNameText->SetText(FText::FromString(InName + Suffix));
	}
}

void UHBSteamFriendEntryWidget::HandleInviteClicked()
{
	OnInviteClicked.Broadcast(NetIdStr);
}
