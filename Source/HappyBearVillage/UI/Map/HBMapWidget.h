// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HBMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UHBMapWidget(const FObjectInitializer& ObjectInitializer);
	
	void SetMapTexture(UTexture2D* Texture);
	void SetPlayerPosition(FVector NormalizedLocation);
	void SetPlayerDirAngle(float Angle);

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SpawnMark(FLinearColor Color, const FVector2D& NormalizedPosition);

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Map")
	TObjectPtr<class UImage> Map;

	UPROPERTY(VisibleAnywhere, Category = "Map")
	TObjectPtr<UMaterialInstanceDynamic> MapDynamicMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Map Mark")
	TMap<FLinearColor, class UUserWidget*> MarksByColor;

	UPROPERTY(VisibleAnywhere, Category = "Map Mark")
	TSubclassOf<class UUserWidget> MarkWidgetClass;
};
