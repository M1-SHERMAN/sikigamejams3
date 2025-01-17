// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class AUGameHUD;
class APlayerCharacter;
class APlayerPawn;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
UCLASS()
class SIKIGAMEJAMS3_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetHUDAlertLevel(float AlertLevel, float MaxAlertLevel);
	void SetHUDSatiety(float Satiety, float MaxSatiety);
	void SetHUDRangeAttackTime(int InRangeAttackTime);
	void SetHUDRemainingEnemies(int InRemainingEnemies);
	
protected:
	virtual void BeginPlay() override;
	void TryGetPlayerCharacter();
	
private:
	UPROPERTY(EditAnywhere, Category = "Enhanced Input Assets")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, Category = "Enhanced Input Assets")
	UInputAction* Movement;
	UPROPERTY(EditAnywhere, Category = "Enhanced Input Assets")
	UInputAction* Look;
	UPROPERTY(EditAnywhere, Category = "Enhanced Input Assets")
	UInputAction* Aim;
	UPROPERTY(EditAnywhere, Category = "Enhanced Input Assets")
	UInputAction* Attack;
	UPROPERTY(EditAnywhere, Category = "Enhanced Input Assets")
	UInputAction* Interact;
	UPROPERTY(EditAnywhere, Category = "Enhanced Input Assets")
	UInputAction* Jump;
	UPROPERTY(EditAnywhere, Category = "Enhanced Input Assets")
	UInputAction* Sprint;

	void OnMovementTriggered(const FInputActionValue &Value);
	void OnLookTriggered(const FInputActionValue &Value);
	void OnAimTriggered(const FInputActionValue &Value);
	void OnAimCompleted(const FInputActionValue &Value);
	void OnAttackTriggered(const FInputActionValue &Value);
	void OnAttackCompleted(const FInputActionValue &Value);
	void OnInteractStarted(const FInputActionValue &Value);
	void OnJumpStarted(const FInputActionValue &Value);
	void OnSprintTriggered(const FInputActionValue &Value);
	void OnSprintCompleted(const FInputActionValue &Value);

	UPROPERTY()
	APlayerCharacter* OwningPlayerCharacter;

	UPROPERTY()
	AUGameHUD* GameHUD;
};
