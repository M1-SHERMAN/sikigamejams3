// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

class UGameOverlay;
/**
 * 
 */
UCLASS()
class SIKIGAMEJAMS3_API AUGameHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UGameOverlay* GameOverlay;
	
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category="Game Overlay")
	TSubclassOf<UGameOverlay> GameOverlayClass;

	void AddGameOverlay();
};
