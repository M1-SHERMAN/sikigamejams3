// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractWithEnmyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractWithEnmyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIKIGAMEJAMS3_API IInteractWithEnmyInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="InteractWithEnemy")
	void GettingAttack();
};
