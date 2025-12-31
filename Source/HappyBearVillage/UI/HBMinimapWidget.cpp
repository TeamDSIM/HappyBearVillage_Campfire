// Fill out your copyright notice in the Description page of Project Settings.


#include "HBMinimapWidget.h"
#include "Components/Image.h"

void UHBMinimapWidget::SetMinimapTexture(UTexture2D* Texture)
{
	MinimapDynamicMaterial->SetTextureParameterValue(FName("MinimapTexture"), Texture);
}

void UHBMinimapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UMaterialInstance* MinimapMaterial = LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/UI/Minimap/MI_MinimapMaterial.MI_MinimapMaterial"));
	MinimapDynamicMaterial = UMaterialInstanceDynamic::Create(MinimapMaterial, this);
	Minimap->SetBrushFromMaterial(MinimapDynamicMaterial);
}
