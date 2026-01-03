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
	void SetMapTexture(UTexture2D* Texture);
	void SetPlayerPosition(FVector NormalizedLocation);
	void SetPlayerDirAngle(float Angle);

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SpawnMarkAtLocalPosition(const FVector2D& LocalPosition);

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Map")
	TObjectPtr<class UImage> Map;

	UPROPERTY(VisibleAnywhere, Category = "Map")
	TObjectPtr<UMaterialInstanceDynamic> MapDynamicMaterial;
};
