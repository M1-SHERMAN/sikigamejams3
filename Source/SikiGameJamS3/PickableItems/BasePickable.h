// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickable.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class SIKIGAMEJAMS3_API ABasePickable : public AActor
{
	GENERATED_BODY()
	
public:	
	ABasePickable();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category="Pickable|Properties")
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere, Category="Pickable|Properties")
	UBoxComponent* CollisionBox;

	UFUNCTION()
	virtual void OnBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
