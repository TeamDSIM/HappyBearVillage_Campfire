// Fill out your copyright notice in the Description page of Project Settings.


#include "HBSteamFriendEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UHBSteamFriendEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InviteButton)
	{
		// HandleInviteClicked 호출
		InviteButton->OnClicked.AddDynamic(this, &ThisClass::HandleInviteClicked);
	}
}

void UHBSteamFriendEntryWidget::Init(const FString& InName, const FString& InNetIdStr, bool bOnline)
{
	// 1. 나중에 버튼 누를 때 누구를 초대해야 하는지 기억
	NetIdStr = InNetIdStr;

	// 2. 텍스트 표시
	if (FriendNameText)
	{
		const FString Suffix = bOnline ? TEXT(" (Online)") : TEXT(" (Offline)");
		FriendNameText->SetText(FText::FromString(InName + Suffix));
	}
}

void UHBSteamFriendEntryWidget::HandleInviteClicked()
{
	// 버튼 눌렀을 때, 이 엔트리가 가진 NetIdStr을 밖으로 알림
	// (로비 위젯이 이 델리게이트를 구독하고 있어서 거기로 전달됨)
	OnInviteClicked.Broadcast(NetIdStr);
}
