// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Status/HBHealthSlot.h"
#include "Components/Image.h"

void UHBHealthSlot::SetImage(UTexture2D* Texture)
{
	if (!Heart || !Texture) return;

	Heart->SetBrushFromTexture(Texture, false);
}
