// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HBTitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API AHBTitlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHBTitlePlayerController();

	virtual void BeginPlay() override;

	void ResetUI();
	void SetupUI();
	void RemoveUI();

	void CreateTitleUI();

protected:
	//강타입의 경우는 지정한 위젯만 들어올 수 있고 (로비 UI, 인게임 HUD)
	//약타입의 경우는 어느 위젯이든 가리킬 수 있음 (타이틀/팝업/오버레이)
	//보통 Class는 약타입, Instance는 강타입으로 사용


	//강 타입 참조
	UPROPERTY()
	class UHBTitleWidget* TitleWidget;

	//약타입의 경우 다음과 같이 선언함 (userwidget을 상속한 어느 위젯이든 가리킬 수 있음)
	//UPROPERTY()
	//UUserWidget* LobbyWidget = nullptr;

	//약 타입 참조
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> TitleWidgetClass;

	//강타입의 경우 다음과 같이 선언함 (HBTitleWidget만 들어올 수 있음)
	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<UHBTitleWidget> LobbyWidgetClass;

	//생성한 UI  목록
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> SpawnedWidgets;
};
