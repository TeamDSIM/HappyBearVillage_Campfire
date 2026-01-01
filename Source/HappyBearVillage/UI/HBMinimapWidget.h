// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HBMinimapWidget.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBMinimapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMinimapTexture(UTexture2D* Texture);
	void SetPlayerPosition(FVector NormalizedLocation);
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Minimap")
	TObjectPtr<class UImage> Minimap;
	
	UPROPERTY(VisibleAnywhere, Category = "Minimap")
	TObjectPtr<UMaterialInstanceDynamic> MinimapDynamicMaterial;
};
