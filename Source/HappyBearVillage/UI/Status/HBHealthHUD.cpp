// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Status/HBHealthHUD.h"

#include "HappyBearVillage.h"
#include "HBHealthSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"


void UHBHealthHUD::UpdateHealth(int32 NewHealth)
{
	UE_LOG(LogHY, Log, TEXT("UpdateHealth %d"), NewHealth);
	
	if (!HeartSlots)
	{
		UE_LOG(LogHY, Log, TEXT("HeartSlots Null"));
		return;
	}
	if (!HealthSlotClass)
	{
		UE_LOG(LogHY, Log, TEXT("HeartSlotClass Null"));
		return;
	}

	HeartSlots->ClearChildren();

	for (int32 i = 0; i < NewHealth; ++i)
	{
		UHBHealthSlot* HBHealthSlot = CreateWidget<UHBHealthSlot>(GetWorld(), HealthSlotClass);
		if (!HBHealthSlot)
		{
			continue;
		}
		
		HBHealthSlot->SetImage(HeartTexture);

		UHorizontalBoxSlot* BoxSlot = HeartSlots->AddChildToHorizontalBox(HBHealthSlot);
		if (BoxSlot)
		{
			BoxSlot->SetPadding(FMargin(8.f, 0.f));
			BoxSlot->SetHorizontalAlignment(HAlign_Left);
			BoxSlot->SetVerticalAlignment(VAlign_Center);
		}
	}
}
