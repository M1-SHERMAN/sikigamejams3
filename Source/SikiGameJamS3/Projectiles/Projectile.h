// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UParticleSystemComponent;
class UParticleSystem;
class USoundCue;

UCLASS()
class SIKIGAMEJAMS3_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;
	
	UPROPERTY()
	UParticleSystemComponent* ParticleSystemComponent;
	UPROPERTY(EditAnywhere)
	UParticleSystem* TraceParticle;
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticle;
	
	UPROPERTY(EditAnywhere, Category="Audio")
	USoundCue* ImpactSound;
	UPROPERTY(EditAnywhere, Category="Audio")
	USoundCue* LaunchSound;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		FVector NormalImpulse,
		const FHitResult& Hit);
public:	
	FORCEINLINE USoundCue* GetLaunchSound() const { return LaunchSound; }

};
