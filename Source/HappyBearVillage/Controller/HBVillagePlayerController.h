// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HBVillagePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API AHBVillagePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHBVillagePlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	//관전 관련 코드
	void EnterObserveMode();
	void ExitObserveMode();
	
protected:
	void ToggleMapWidget();

	//관전 관련 코드
	class AHBCharacterPlayer* FindAnyAlivePlayer(UWorld* World, AActor* ExcludeActor);

	void ObserveToAnyAlivePlayer();
	void BuildObserveCandidates();
	void ApplyObserveTarget(int32 NewIndex, bool bBlend);
	void ObserveNext();
	void ObservePrev();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> PlayerIMC;
	
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> ToggleMapAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> ObserveIMC; 

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> ObserveNextInputAction; // D

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> ObservePrevInputAction; // A

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<class UHBInGameHUDComponent> InGameHUDComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<class UHBMinimapWidgetComponent> MinimapWidgetComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<class UHBMapWidgetComponent> MapWidgetComponent;

	//관전 가능 플레이어 목록
	//Weak로 선언하는 이유: 플레이어가 죽으면 자동으로 무효화
	UPROPERTY()
	TArray<TWeakObjectPtr<AHBCharacterPlayer>> ObserveCandidates;

	int32 ObserveIndex = INDEX_NONE;
	bool bIsObserving = false;
};
