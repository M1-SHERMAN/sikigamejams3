// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "SikiGameJamS3/Interfaces/InteractWithEnmyInterface.h"
#include "PlayerCharacter.generated.h"

class UBoxComponent;
class UAnimMontage;
class USpringArmComponent;
class UCameraComponent;
class USoundCue;
UCLASS()
class SIKIGAMEJAMS3_API APlayerCharacter : public ABaseCharacter, public IInteractWithEnmyInterface
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
	 * Attack Properties
	 */
	UPROPERTY(EditAnywhere, Category="Attack|CollisionBox")
	UBoxComponent* MeleeAttackCollisionBox;
	UFUNCTION(BlueprintCallable, Category="Attack|Melee")
	void OnMeleeAttackCollisionBoxOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UPROPERTY(EditAnywhere, Category="Attack|Range")
	TSubclassOf<class AProjectile> ProjectileClass;
	
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
	bool bIsInterpolatingSatiety;
	float TargetSatiety;
	

	
	/**
	 * Character Data
	 */
	int RangeAttackTimes = 0;
	float SpeedFactor;

	UPROPERTY(EditAnywhere, Category="Character|Movement Datas")
	float SprintSpeed = 600.f;
	UPROPERTY(EditAnywhere, Category="Character|Movement Datas")
	float WalkSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category="Character|Movement Datas")
	float AimSpeed = 150.f;
	UPROPERTY(EditAnywhere, Category="Character|Movement Datas")
	float MinSpeedFactor = 0.2f;
	UPROPERTY(EditAnywhere, Category="Character|Movement Datas")
	float MaxSpeedFactor = 1.2f;
	UPROPERTY(EditAnywhere, Category="Character|Movement Datas")
	float LowerSatietyEffectThreshold = 0.2f;
	UPROPERTY(EditAnywhere, Category="Character|Movement Datas")
	float UpperSatietyEffectThreshold = 0.95f;
	
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Basic")
	float CurrentAlertLevel = 0.f;
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Basic")
	float MaxAlertLevel = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Attack")
	float MeleeAttackAlertLevel = 15.f;
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Attack")
	float RangeAttackAlertLevel = 5.f;
	
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Movement")
	float SprintAlertLevel = 0.02f;
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Movement")
	float WalkAlertLevel = 0.009f;
	
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Decay")
	float AlertLevelDecayRate = 1.f;
	UPROPERTY(EditAnywhere, Category="Character|Alert Datas|Decay")
	float AlertLevelNatualDecayThreshold = 50.f;
	
	UPROPERTY(EditAnywhere, Category="Character|Satiety Datas")
	float Satiety = 50.f;
	UPROPERTY(EditAnywhere, Category="Character|Satiety Datas")
	float MaxSatiety = 100.f;
	UPROPERTY(EditAnywhere, Category="Character|Satiety Datas")
	float SatietyDecayRate = 0.7f;
	UPROPERTY(EditAnywhere, Category="Character|Satiety Datas")
	float SatietyDecayFactorWalk = 1.2f;
	UPROPERTY(EditAnywhere, Category="Character|Satiety Datas")
	float SatietyDecayFactorSprint = 1.5f;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSatietyChanged, float, SatietyValue);
	FTimerHandle SatietyChangeBroadcastTimerHandle;

	
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void SetCharacterFollowCamera(bool bWillFollowCamera);
	void UpdateMeshScaleAndSpringArmLocation(float InSatiety, float InMaxSatiety);

	/**
	 * Line Trace: mainly for calculating the projectile eject location
	 */
	FVector ProjectileEjectLocation;
	FRotator ProjectileEjectRotation;
	FHitResult LineTraceHitResult;
	void DoLineTrace(FVector& OutEjectLocation, FRotator& OutEjectRotator);

	/**
	 * Aim Dot
	 */
	UPROPERTY(EditAnywhere, Category = "Aim Dot")
	UTexture2D* AimDot;
	UPROPERTY(EditAnywhere, Category = "Aim Dot")
	FLinearColor AimDotColor = FLinearColor::White;

	/**
	 * Audio
	 */
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* MeleeAttackSound;
	UFUNCTION()
	void PlayMeleeAttackSound();
public:
	void UpdateAlertLevel(float DeltaSeconds, float InAlertLevel, bool bIsNaturalDecay = false);
	void UpdateSatiety(float DeltaSeconds, float InSatiety, bool bIsDecay = false);
	void UpdateSpeed(float InSpeed, float InSpeedFactor = 1.f);
	float CalcSpeedFactor(float InSatiety, float InMaxSatiety);

	UFUNCTION(BlueprintCallable)
	void SetSatiety(float InSatiety) { Satiety = InSatiety; }
	FOnSatietyChanged OnSatietyChanged;
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetSatiety() const { return Satiety; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetAlertLevel() const { return CurrentAlertLevel; }
	
	
	FORCEINLINE void IncreaseRangeAttackTimes(int InRangeAttackTimes) { RangeAttackTimes += InRangeAttackTimes; }
};
