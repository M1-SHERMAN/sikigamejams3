// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UAnimMontage;
class USpringArmComponent;
class UCameraComponent;
UCLASS()
class SIKIGAMEJAMS3_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	APlayerCharacter();
	virtual void Tick(float DeltaSeconds) override;
	
	void HandleMovement(const FVector2D& InputValue);
	void HandleLook(const FVector2D& InputValue);
	void HandleAim(bool bInIsAiming);
	void HandleAttack(bool bInIsAttackStart);
	void HandleInteract(bool bInIsInteracting);
	void HandleJump();
	void HandleSprint(bool bInIsSprinting);

	void UpdateHUD();
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	class AMyPlayerController* MyPlayerController;
	/**
	 * Camera Properties
	 */
	UPROPERTY(VisibleAnywhere, Category="Camera")
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, Category="Camera")
	UCameraComponent* CameraComponent;

	/**
	 * Animation Montages
	 */
	UPROPERTY(EditAnywhere, Category="Montage|Attack")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, Category="Montage|Attack")
	UAnimMontage* AimMontage;
	UPROPERTY(EditAnywhere, Category="Montage|Interaction")
	UAnimMontage* InteractionMontage;

	/**
	* Character State
	*/
	bool bIsAiming;
	bool bIsAttackStart;
	bool bIsInteracting;
	bool bIsSprinting;

	UPROPERTY(EditAnywhere, Category="Character|State")
	int RangeAttackTimes = 0;

	UPROPERTY(EditAnywhere, Category="Character|Movement Datas")
	float SprintSpeed = 600.f;
	UPROPERTY(EditAnywhere, Category="Character|Movement Datas")
	float WalkSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category="Character|Movement Datas")
	float AimSpeed = 150.f;
	
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Basic")
	float CurrentAlertLevel = 0.f;
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Basic")
	float MaxAlertLevel = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Attack")
	float MeleeAttackAlertLevel = 100.f;
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Attack")
	float RangeAttackAlertLevel = 10.f;
	
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Movement")
	float SprintAlertLevel = 0.09f;
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Movement")
	float WalkAlertLevel = 0.04f;
	
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Decay")
	float AlertLevelDecayRate = 1.f;
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Decay")
	float AlertLevelNatualDecayThreshold = 70.f;
	
	UPROPERTY(EditAnywhere, Category="Character|Satiety Datas")
	float Satiety = 100.f;
	UPROPERTY(EditAnywhere, Category="Character|Satiety Datas")
	float MaxSatiety = 100.f;
	UPROPERTY(EditAnywhere, Category="Character|Satiety Datas")
	float SatietyDecayRate = 1.f;

	void SetCharacterFollowCamera(bool bWillFollowCamera);
public:
	void UpdateAlertLevel(float DeltaSeconds, float InAlertLevel, bool bIsNaturalDecay = false);
	void UpdateSatiety(float DeltaSeconds, float InHungerLevel, bool bIsNaturalDecay = false);
	
	FORCEINLINE void IncreaseRangeAttackTimes(int InRangeAttackTimes) { RangeAttackTimes += InRangeAttackTimes; }
};
