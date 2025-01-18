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
	virtual void DrawHUD() override;

	
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category="Game Overlay")
	TSubclassOf<UGameOverlay> GameOverlayClass;

	void AddGameOverlay();

	UPROPERTY()
	UTexture2D* AimDot;
	FLinearColor AimDotColor;
	void DrawAimDot(UTexture2D* AimDotTexture,
		const FVector2D& ViewportCenter,
		FLinearColor Color);
public:
	FORCEINLINE UTexture2D* GetAimDot() const { return AimDot; }
	FORCEINLINE void SetAimDot(UTexture2D* InAimDot) { AimDot = InAimDot; }
	FORCEINLINE void SetAimDotColor(FLinearColor InAimDotColor) { AimDotColor = InAimDotColor; }

};
