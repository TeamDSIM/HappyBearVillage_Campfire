// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Status/HBHealthHUD.h"

#include "HBHealthSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void UHBHealthHUD::UpdateHealth(int32 NewHealth)
{
	if (!HeartSlots)
	{
		return;
	}
	if (!HealthSlotClass)
	{
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

		UHorizontalBoxSlot* BoxSlot = HeartSlots->AddChildToHorizontalBox(HBHealthSlot);
		if (BoxSlot)
		{
			BoxSlot->SetPadding(FMargin(8.f, 0.f));
		}
	}
}
