// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickable.h"
#include "PickableDecreaseAlert.generated.h"

/**
 * 
 */
UCLASS()
class SIKIGAMEJAMS3_API APickableDecreaseAlert : public ABasePickable
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
	virtual void OnBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
private:
	UPROPERTY(EditAnywhere, Category="Pickable|Effects")
	float AlertDecreaseValue = 10.0f;
};
