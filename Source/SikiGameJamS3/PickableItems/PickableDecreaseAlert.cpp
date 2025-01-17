// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableDecreaseAlert.h"

#include "SikiGameJamS3/Characters/PlayerCharacter.h"

void APickableDecreaseAlert::BeginPlay()
{
	Super::BeginPlay();
}

void APickableDecreaseAlert::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->UpdateAlertLevel(0.0f, -AlertDecreaseValue, false);
		Destroy();
	}
}
