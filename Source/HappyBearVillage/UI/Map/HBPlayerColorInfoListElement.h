// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "HBPlayerColorInfoListElement.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class HAPPYBEARVILLAGE_API UHBPlayerColorInfoListElement : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> PlayerColorInfo;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerName;
};
