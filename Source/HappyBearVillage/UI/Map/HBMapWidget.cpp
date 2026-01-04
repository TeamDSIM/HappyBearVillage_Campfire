// Fill out your copyright notice in the Description page of Project Settings.


#include "HBMapWidget.h"

#include "HappyBearVillage.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

UHBMapWidget::UHBMapWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MarkWidgetClassRef(TEXT("/Game/UI/Map/WBP_MapMark.WBP_MapMark_C"));
	if (MarkWidgetClassRef.Succeeded())
	{
		MarkWidgetClass = MarkWidgetClassRef.Class;
	}
}

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
	FVector2D LocalLocation = ImageGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	FVector2D ImageSize = ImageGeometry.GetLocalSize();
	FVector2D NormalizedLocation(LocalLocation.X / ImageSize.X, LocalLocation.Y / ImageSize.Y);

	if (LocalLocation.X < 0 || LocalLocation.Y < 0 || LocalLocation.X > ImageSize.X || LocalLocation.Y > ImageSize.Y)
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}
	
	UE_LOG(LogYS, Log, TEXT("Local Location : %f, %f"), LocalLocation.X, LocalLocation.Y);
	UE_LOG(LogYS, Log, TEXT("Normalized Location : %f, %f"), NormalizedLocation.X, NormalizedLocation.Y);
	SpawnMarkAtLocalPosition(FLinearColor::Black, NormalizedLocation);
	
	return FReply::Handled();
}

void UHBMapWidget::SpawnMarkAtLocalPosition(FLinearColor Color, const FVector2D& NormalizedLocation)
{
	FGeometry ImageGeometry = Map->GetCachedGeometry();
	FVector2D ImageSize = ImageGeometry.GetLocalSize();

	FVector2D ImageOffset = ImageGeometry.AbsoluteToLocal(ImageGeometry.GetAbsolutePosition());
	FVector2D MarkLocalPosition = ImageOffset + FVector2D(NormalizedLocation.X * ImageSize.X, NormalizedLocation.Y * ImageSize.Y);

	FVector2D Test1 = ImageGeometry.GetAbsolutePosition();
	
	UE_LOG(LogYS, Log, TEXT("ImageOffset : %f, %f"), ImageOffset.X, ImageOffset.Y);
	UE_LOG(LogYS, Log, TEXT("MarkLocalPosition : %f, %f"), MarkLocalPosition.X, MarkLocalPosition.Y);
	
	UUserWidget** Found = MarksByColor.Find(Color);

	if (Found && *Found)
	{
		UUserWidget* Mark = *Found;
		UCanvasPanelSlot* MarkSlot = Cast<UCanvasPanelSlot>(Mark->Slot);
		
		MarkSlot->SetPosition(MarkLocalPosition);
	}
	else
	{
		UUserWidget* NewMark = WidgetTree->ConstructWidget<UUserWidget>(MarkWidgetClass);
		if (!NewMark) return;

		NewMark->SetVisibility(ESlateVisibility::Visible);
		NewMark->SetColorAndOpacity(Color);

		if (UCanvasPanel* ParentCanvas = Cast<UCanvasPanel>(Map->GetParent()))
		{
			UCanvasPanelSlot* NewMarkSlot = ParentCanvas->AddChildToCanvas(NewMark);
			NewMarkSlot->SetAutoSize(false);
			NewMarkSlot->SetSize(FVector2D(8.f, 8.f));
			NewMarkSlot->SetPosition(MarkLocalPosition);
		}
		
		MarksByColor.Add(Color, NewMark);
	}
}
