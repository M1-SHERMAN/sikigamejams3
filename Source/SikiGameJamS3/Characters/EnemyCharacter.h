// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "SikiGameJamS3/Interfaces/InteractWithEnmyInterface.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Patrol,
	RunAway,
	Dead,
	_MAX
};

class UAIPerceptionComponent;
class UAISenseConfig;
class UCapsuleComponent;
class UAudioComponent;
class USoundCue;
UCLASS()
class SIKIGAMEJAMS3_API AEnemyCharacter : public ABaseCharacter, public IInteractWithEnmyInterface
{
	GENERATED_BODY()
public:
	AEnemyCharacter();
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	
	UPROPERTY(EditAnywhere, Category="Enemy|AI")
	UAIPerceptionComponent* AIPerception;

	virtual void GettingAttack_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy|Datas")
	EEnemyState EnemyState = EEnemyState::Patrol;
	UPROPERTY(EditAnywhere, Category="Enemy|Datas")
	float RunAwayDistance = 1000.f;
	UPROPERTY(EditAnywhere, Category="Enemy|Datas")
	float PatrolSpeed = 175.f;
	UPROPERTY(EditAnywhere, Category="Enemy|Datas")
	float RunAwaySpeed = 425.f;
	UPROPERTY(EditAnywhere, Category="Enemy|Datas")
	float SatietyToRecover = 15.f;

	UPROPERTY(EditAnywhere, Category="Enemy|Montages")
	UAnimMontage* GettingAttackMontage;
	UPROPERTY(EditAnywhere, Category="Enemy|Montages")
	UAnimMontage* DeadMontage;
	
	void PlayMontage(UAnimMontage* MontageToPlay);
	UFUNCTION(BlueprintCallable, Category="Enemy|Audio")
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void EatenByPlayerCharacter(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category="Enemy|Audio")
	UAudioComponent* DeadSound;
	UPROPERTY(EditAnywhere, Category="Enemy|Audio")
	USoundCue* EatenSound;
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetEnemyState(EEnemyState InEnemyState) { EnemyState = InEnemyState; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EEnemyState GetEnemyState() const { return EnemyState; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetRunAwayDistance() const { return RunAwayDistance; }
	UFUNCTION(BlueprintCallable)
	float GetCurrentMaxSpeed(bool bIsRunningAway) const;
};
