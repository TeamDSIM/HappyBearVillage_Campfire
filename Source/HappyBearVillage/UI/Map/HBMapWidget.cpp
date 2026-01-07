// Fill out your copyright notice in the Description page of Project Settings.


#include "HBMapWidget.h"

#include "HappyBearVillage.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "GameFramework/GameStateBase.h"
#include "PlayerState/HBPlayerState.h"

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

void UHBMapWidget::RefreshMapMarks(TArray<FHBMapMarkInfo> MapMarks)
{
	UE_LOG(LogYS, Log, TEXT("Refresh Map Mark"));
	ClearMapMarks();

	for (auto& MapMark : MapMarks)
	{
		SpawnMark(MapMark.Color, MapMark.Position);	
	}
}

void UHBMapWidget::ClearMapMarks()
{
	for (const TPair<FLinearColor, UUserWidget*>& MarkPair : MarksByColor)
	{
		MarkPair.Value->RemoveFromParent();
	}

	MarksByColor.Empty();
}

void UHBMapWidget::RefreshPlayerColorList()
{
	for (APlayerState* HBPlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		PlayerColorInfoList->AddItem(HBPlayerState);
	}
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
	FVector2D NormalizedPosition(LocalPosition.X / ImageSize.X, LocalPosition.Y / ImageSize.Y);

	if (LocalPosition.X < 0 || LocalPosition.Y < 0 || LocalPosition.X > ImageSize.X || LocalPosition.Y > ImageSize.Y)
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}

	OnClickMap.Broadcast(OwnMarkColor, NormalizedPosition);
	UE_LOG(LogYS, Log, TEXT("OnClickMap"));
	
	return FReply::Handled();
}

void UHBMapWidget::SpawnMark(FLinearColor Color, const FVector2D& NormalizedPosition)
{
	FGeometry MapImageGeometry = Map->GetCachedGeometry();
	FVector2D MapImageSize = MapImageGeometry.GetLocalSize();
	FVector2D MarkLocalPosition = FVector2D((NormalizedPosition.X - 0.5f) * MapImageSize.X, (NormalizedPosition.Y - 0.5f) * MapImageSize.Y);
	
	UUserWidget** Found = MarksByColor.Find(Color);

	if (!Found || !*Found)
	{
		UUserWidget* NewMark = WidgetTree->ConstructWidget<UUserWidget>(MarkWidgetClass);
		if (!NewMark) return;
		
		NewMark->SetVisibility(ESlateVisibility::Visible);
		NewMark->SetColorAndOpacity(Color);

		UCanvasPanel* MapCanvas = Cast<UCanvasPanel>(Map->GetParent());
		UCanvasPanelSlot* NewMarkSlot = MapCanvas->AddChildToCanvas(NewMark);
		
		NewMarkSlot->SetAutoSize(false);
		NewMarkSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		NewMarkSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		NewMarkSlot->SetPosition(MarkLocalPosition);
		
		MarksByColor.Add(Color, NewMark);
	}
	else
	{
		UUserWidget* Mark = *Found;
		UCanvasPanelSlot* MarkSlot = Cast<UCanvasPanelSlot>(Mark->Slot);

		MarkSlot->SetPosition(MarkLocalPosition);
	}
}
