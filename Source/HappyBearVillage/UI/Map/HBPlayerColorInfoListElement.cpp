// Fill out your copyright notice in the Description page of Project Settings.


#include "HBPlayerColorInfoListElement.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PlayerState/HBPlayerState.h"

void UHBPlayerColorInfoListElement::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const AHBPlayerState* HBPlayerState = Cast<AHBPlayerState>(ListItemObject); 
	if (!HBPlayerState) return;

	PlayerColorInfo->SetColorAndOpacity(HBPlayerState->GetPlayerColor());
	PlayerName->SetText(FText::FromString(HBPlayerState->GetPlayerName()));
}
