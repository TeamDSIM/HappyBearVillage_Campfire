// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HBMinimapWidgetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HAPPYBEARVILLAGE_API UHBMinimapWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHBMinimapWidgetComponent();

	void CreateMinimapWidget(APlayerController* InPlayerController);
	void ShowMinimapWidget();
	void HideMinimapWidget();
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetMinimapTexture();
	void SetPlayerPosition();
	void SetPlayerDirection();

private:
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TSubclassOf<class UHBMinimapWidget> MinimapWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<class UHBMinimapWidget> MinimapWidget;
};
