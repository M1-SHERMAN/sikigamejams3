// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickable.generated.h"

class UWidgetComponent;
class UTextRenderComponent;
class UBoxComponent;
class UStaticMeshComponent;
class USoundCue;

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
	UPROPERTY(EditAnywhere, Category="Pickable|Properties")
	USoundCue* PickSound;
	UPROPERTY(VisibleAnywhere, Category="Pickable|Properties")
	UBoxComponent* ShowTipsBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickable|Properties")
	UWidgetComponent* TipsTextWidget;

	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnTipsBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnTipsBoxEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
