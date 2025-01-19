// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "SikiGameJamS3/Framework/MainGameMode.h"
#include "Sound/SoundCue.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	DeadSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DeadSound"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEnemyCharacter::GettingAttack_Implementation()
{
	IInteractWithEnmyInterface::GettingAttack_Implementation();
	PlayMontage(GettingAttackMontage);
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->UnPossess();
	}
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::EatenByPlayerCharacter);
}

void AEnemyCharacter::EatenByPlayerCharacter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor
		&& OtherActor->GetClass() != GetClass()
		&& OtherActor->Implements<UInteractWithEnmyInterface>())
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
		{
			PlayerCharacter->UpdateSatiety(GetWorld()->GetDeltaSeconds(), SatietyToRecover, false);			
		}
		if (AMainGameMode* MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			MainGameMode->OnEnemyDestroyed();
			Destroy();
		}
		if (EatenSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), EatenSound, GetActorLocation());
		}
	}
}


void AEnemyCharacter::PlayMontage(UAnimMontage* MontageToPlay)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && MontageToPlay)
	{
		AnimInstance->OnMontageEnded.RemoveAll(this);
		AnimInstance->Montage_Play(MontageToPlay);

		if (EnemyState != EEnemyState::Dead && MontageToPlay != DeadMontage)
		{
			AnimInstance->OnMontageBlendingOut.AddDynamic(this, &AEnemyCharacter::OnMontageBlendingOut);
		}
	}
}

void AEnemyCharacter::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (EnemyState != EEnemyState::Dead)
	{
		SetEnemyState(EEnemyState::Dead);
		PlayMontage(DeadMontage);
		if (DeadSound)
		{
			DeadSound->Play();
		}
	}
}

float AEnemyCharacter::GetCurrentMaxSpeed(bool bIsRunningAway) const
{
	if (bIsRunningAway)
	{
		return RunAwaySpeed;
	}
	else
	{
		return PatrolSpeed;
	}
	
}
