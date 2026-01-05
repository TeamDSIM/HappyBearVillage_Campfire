// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HBMapWidgetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HAPPYBEARVILLAGE_API UHBMapWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHBMapWidgetComponent();

	FORCEINLINE bool IsMapVisible() const { return bIsMapVisible; }
	bool IsMapValid() const ;

	void CreateMapWidget(APlayerController* InPlayerController);
	void ShowMapWidget();
	void HideMapWidget();
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetMapTexture();
	void SetPlayerPosition();
	void SetPlayerDirection();
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TSubclassOf<class UHBMapWidget> MapWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<class UHBMapWidget> MapWidget;

	UPROPERTY(VisibleAnywhere, Category = "Widget Info")
	uint8 bIsMapVisible : 1;
};
