// Fill out your copyright notice in the Description page of Project Settings.


#include "HBMinimapWidget.h"
#include "Components/Image.h"

void UHBMinimapWidget::SetMinimapTexture(UTexture2D* Texture)
{
	MinimapDynamicMaterial->SetTextureParameterValue(FName("MinimapTexture"), Texture);
}

void UHBMinimapWidget::SetPlayerPosition(FVector NormalizedLocation)
{
	MinimapDynamicMaterial->SetVectorParameterValue(FName("PlayerPos"), FLinearColor(NormalizedLocation));
}

void UHBMinimapWidget::SetPlayerDirAngle(float Angle)
{
	MinimapDynamicMaterial->SetScalarParameterValue(FName("PlayerDirAngle"), Angle);
}

void UHBMinimapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UMaterialInstance* MinimapMaterial = LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/UI/Map/MI_MinimapMaterial.MI_MinimapMaterial"));
	MinimapDynamicMaterial = UMaterialInstanceDynamic::Create(MinimapMaterial, this);
	Minimap->SetBrushFromMaterial(MinimapDynamicMaterial);
}
