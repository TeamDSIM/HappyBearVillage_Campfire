// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HBUserWidget.h"
#include "HBPlayerRoleWidget.generated.h"

enum class EJobType : uint8;
enum class ERoleType : uint8;
/**
 * 
 */
UCLASS()
class HAPPYBEARVILLAGE_API UHBPlayerRoleWidget : public UHBUserWidget
{
	GENERATED_BODY()

public:
	UHBPlayerRoleWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdatePlayerRole(ERoleType NewRole);
	void UpdatePlayerJob(EJobType NewJob);

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> RoleText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> JobText;

	UPROPERTY()
	FString PlayerRole;

	UPROPERTY()
	FString PlayerJob;
};
