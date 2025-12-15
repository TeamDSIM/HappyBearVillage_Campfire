// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HBInteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHBInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	작성일: 2025-12-15
 *  작성자: 박현용
 *  설명: 상호작용 가능한 오브젝트에 공통으로 들어갈 기능 선언
 *			CanInteract(AActor* Interactor) : Interactor 와 상호작용 가능 여부 반환
 *			Interact(AActor* Interactor) : Interactor 와 실제 상호작용 기능 구현
 */
class HAPPYBEARVILLAGE_API IHBInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CanInteract(AActor* Interactor) const = 0;
	virtual void Interact(AActor* Interactor) = 0;
};
