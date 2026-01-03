// Fill out your copyright notice in the Description page of Project Settings.


#include "HBMapWidget.h"

#include "HappyBearVillage.h"
#include "Components/Image.h"

void UHBMapWidget::SetMapTexture(UTexture2D* Texture)
{
	MapDynamicMaterial->SetTextureParameterValue(FName("MapTexture"), Texture);
}

void UHBMapWidget::SetPlayerPosition(FVector NormalizedLocation)
{
	MapDynamicMaterial->SetVectorParameterValue(FName("PlayerPos"), FLinearColor(NormalizedLocation));
}

void UHBMapWidget::SetPlayerDirAngle(float Angle)
{
	MapDynamicMaterial->SetScalarParameterValue(FName("PlayerDirAngle"), Angle);
}

void UHBMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UMaterialInstance* MapMaterial = LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/UI/Map/MI_MapMaterial.MI_MapMaterial"));
	MapDynamicMaterial = UMaterialInstanceDynamic::Create(MapMaterial, this);
	Map->SetBrushFromMaterial(MapDynamicMaterial);
}

FReply UHBMapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() != EKeys::RightMouseButton)
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);	
	}
	
	FGeometry ImageGeometry = Map->GetCachedGeometry();
	FVector2D LocalPosition = ImageGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	FVector2D ImageSize = ImageGeometry.GetLocalSize();
	
	UE_LOG(LogYS, Log, TEXT("Local Position : %f, %f"), LocalPosition.X, LocalPosition.Y);
	
	if (LocalPosition.X < 0 || LocalPosition.Y < 0 || LocalPosition.X > ImageSize.X || LocalPosition.Y > ImageSize.Y)
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}

	SpawnMarkAtLocalPosition(LocalPosition);
	return FReply::Handled();
}

void UHBMapWidget::SpawnMarkAtLocalPosition(const FVector2D& LocalPosition)
{
	/*
	UImage* NewMark = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	NewMark->SetBrush(ClickableImage->Brush); // 원하면 별도 이미지
	NewMark->SetVisibility(ESlateVisibility::Visible);

	// Overlay에 추가
	if (UOverlay* ParentOverlay = Cast<UOverlay>(ClickableImage->GetParent()))
	{
		UOverlaySlot* Slot = ParentOverlay->AddChildToOverlay(NewMark);
		Slot->SetPadding(FMargin(LocalPos.X, LocalPos.Y, 0, 0));
		Slot->SetHorizontalAlignment(HAlign_Left);
		Slot->SetVerticalAlignment(VAlign_Top);
	}
	*/
}
