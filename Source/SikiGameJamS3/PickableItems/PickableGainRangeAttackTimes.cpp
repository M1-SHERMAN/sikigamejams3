// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableGainRangeAttackTimes.h"

#include "SikiGameJamS3/Characters/PlayerCharacter.h"

class APlayerCharacter;

void APickableGainRangeAttackTimes::BeginPlay()
{
	Super::BeginPlay();
}

void APickableGainRangeAttackTimes::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
											   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->IncreaseRangeAttackTimes(RangeAttackTimes);
		Destroy();
	}
}