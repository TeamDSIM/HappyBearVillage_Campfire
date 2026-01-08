// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HBMapMarkInfo.h"
#include "Blueprint/UserWidget.h"
#include "HBMapWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClickMap, FLinearColor /* Color */, FVector2D /* MarkPosition */);

class UTextBlock;

UCLASS()
class HAPPYBEARVILLAGE_API UHBMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UHBMapWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE void SetOwnMarkColor(FLinearColor InOwnMarKColor) { OwnMarkColor = InOwnMarKColor; }
	
	void SetMapTexture(UTexture2D* Texture);
	void SetPlayerPosition(FVector NormalizedLocation);
	void SetPlayerDirAngle(float Angle);

	void RefreshMapMarks(TArray<FHBMapMarkInfo> MapMarks);
	void ClearMapMarks();

	void RefreshPlayerColorList();

public:
	FOnClickMap OnClickMap;

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SpawnMark(FLinearColor Color, const FVector2D& NormalizedPosition);

	void SetRoleDescText();


protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Map")
	TObjectPtr<class UImage> Map;

	UPROPERTY(VisibleAnywhere, Category = "Map")
	TObjectPtr<UMaterialInstanceDynamic> MapDynamicMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Map Mark")
	TMap<FLinearColor, class UUserWidget*> MarksByColor;

	UPROPERTY(VisibleAnywhere, Category = "Map Mark")
	TSubclassOf<class UUserWidget> MarkWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Map Mark")
	FLinearColor OwnMarkColor = FLinearColor::Black;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Player Color")
	TObjectPtr<class UListView> PlayerColorInfoList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RoleDescText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "JobInfo")
	UDataTable* JobInfoTable;
};
