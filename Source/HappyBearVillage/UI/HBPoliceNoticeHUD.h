// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HBUserWidget.h"
#include "HBPoliceNoticeHUD.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBPoliceNoticeHUD : public UHBUserWidget
{
	GENERATED_BODY()

public:
	UHBPoliceNoticeHUD(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	void SetEffectToolTipWidget();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UHBEffectToolTip> EffectToolTipClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHBEffectToolTip> EffectToolTipWidget;
};
