// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnemyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SikiGameJamS3/Controllers/MyPlayerController.h"
#include "SikiGameJamS3/Framework/MainGameMode.h"
#include "SikiGameJamS3/Projectiles/Projectile.h"
#include "Sound/SoundCue.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	MeleeAttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollisionBox"));
	MeleeAttackCollisionBox->SetupAttachment(GetMesh(), FName("tongue_03"));
	MeleeAttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeleeAttackCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	SetCharacterFollowCamera(false);
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateAlertLevel(DeltaSeconds, AlertLevelDecayRate, true);
	UpdateSatiety(DeltaSeconds, SatietyDecayRate,true);

	if (MeleeAttackCollisionBox && GetMesh())
	{
		MeleeAttackCollisionBox->AttachToComponent(
	GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		FName("tongue_03"));
	}

	if (bIsAiming || bIsAttackStart)
	{
		UpdateSpeed(AimSpeed, SpeedFactor);
	}
	else if (bIsSprinting)
	{
		UpdateSpeed(SprintSpeed, SpeedFactor);
	}
	else
	{
		UpdateSpeed(WalkSpeed, SpeedFactor);
	}
	
	DoLineTrace(ProjectileEjectLocation, ProjectileEjectRotation);
	UpdateHUD();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateHUD();
	OnSatietyChanged.Broadcast(Satiety);

	// Broadcast Satiety change every 3 seconds
	// If Broadcast in UpdateSatiety, it will be broadcast every frame
	GetWorld()->GetTimerManager().SetTimer(
		SatietyChangeBroadcastTimerHandle,
		[this](){OnSatietyChanged.Broadcast(Satiety);},
		3.f,
		true);
}

void APlayerCharacter::OnMeleeAttackCollisionBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor
		&& OtherActor != this
		&& OtherActor->Implements<UInteractWithEnmyInterface>())
	{
		if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor))
		{
			EnemyCharacter->Execute_GettingAttack(OtherActor);
			PlayMeleeAttackSound();
		}
	}
}

void APlayerCharacter::HandleMovement(const FVector2D& InputValue)
{
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, InputValue.Y);
		AddMovementInput(RightDirection, InputValue.X);
		
		float NewAlertLevel = bIsSprinting ? SprintAlertLevel : WalkAlertLevel;
		UpdateAlertLevel(0.0f,NewAlertLevel, false);
		float SatietyDecayFactor = bIsSprinting ? -SatietyDecayFactorSprint : -SatietyDecayFactorWalk;
		float NewSatiety = SatietyDecayFactor * SatietyDecayRate;
		UpdateSatiety(0.0f, NewSatiety, true);
	}
}

void APlayerCharacter::HandleLook(const FVector2D& InputValue)
{
	if (Controller)
	{
		AddControllerYawInput(InputValue.X);
		AddControllerPitchInput(InputValue.Y);
	}
}

void APlayerCharacter::HandleAim(bool bInIsAiming)
{
	if (!Controller
		|| RangeAttackTimes <= 0)
	{
		return;
	}

	bIsAiming = bInIsAiming;
	if (bIsAiming)
	{
		SetCharacterFollowCamera(true);
	}
	else
	{
		SetCharacterFollowCamera(false);
	}
}

void APlayerCharacter::HandleAttack(bool bInIsAttackStart)
{
	if (!Controller)
	{
		return;
	}
	bIsAttackStart = bInIsAttackStart;
	if (bIsAttackStart)
	{
		SetCharacterFollowCamera(true);
	}
	else
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackMontage)
		{
			AnimInstance->OnMontageEnded.RemoveAll(this);
			
			AnimInstance->Montage_Play(AttackMontage);
			FName SectionName;
			float NewInAlertLevel;
			
			if (bIsAiming)
			{
				SectionName = FName("Shoot");

				if (ProjectileClass)
				{
					if (UWorld* World = GetWorld())
					{
						FActorSpawnParameters SpawnParams;
						SpawnParams.Owner = this;
						World->SpawnActor<AProjectile>(
							ProjectileClass,
							ProjectileEjectLocation,
							ProjectileEjectRotation,
							SpawnParams);
					}
				}
				
				NewInAlertLevel = RangeAttackAlertLevel;
				RangeAttackTimes = FMath::Max(0, RangeAttackTimes - 1);

				bIsAiming = false;
			}
			else
			{
				SectionName = FName("Melee");
				NewInAlertLevel = MeleeAttackAlertLevel;
				MeleeAttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				MeleeAttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnMeleeAttackCollisionBoxOverlap);
				MeleeAttackCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
			}
			
			UpdateAlertLevel(0.0f, NewInAlertLevel, false);
			AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
			AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnMontageEnded);
			SetCharacterFollowCamera(false);
		}
	}
}

void APlayerCharacter::HandleInteract(bool bInIsInteracting)
{
	if (!Controller)
	{
		return;
	}
	bIsInteracting = bInIsInteracting;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && bInIsInteracting && InteractionMontage)
	{
		AnimInstance->Montage_Play(InteractionMontage);
	}
}

void APlayerCharacter::HandleJump()
{
	if (!Controller)
	{
		return;
	}
	Jump();
}

void APlayerCharacter::HandleSprint(bool bInIsSprinting)
{
	if (!Controller)
	{
		return;
	}
	bIsSprinting = bInIsSprinting;
}

void APlayerCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
	{
		CharacterMovementComp->MaxWalkSpeed = WalkSpeed * SpeedFactor;
	}
	if (MeleeAttackCollisionBox)
	{
		MeleeAttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeleeAttackCollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		MeleeAttackCollisionBox->OnComponentBeginOverlap.RemoveAll(this);
	}
}

void APlayerCharacter::SetCharacterFollowCamera(bool bWillFollowCamera)
{
	if (bWillFollowCamera)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
	}
}


void APlayerCharacter::DoLineTrace(FVector& OutEjectLocation, FRotator& OutEjectRotator)
{
	FVector2D ViewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}

	FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	if (UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(this,0),
			CrosshairLocation,CrosshairWorldPosition,CrosshairWorldDirection))
	{
		constexpr float TraceLength = 65535.f;
		FVector LineTraceStartPoint = CrosshairWorldPosition;
		float DistanceToCharacter = (GetActorLocation() - LineTraceStartPoint).Size();
		LineTraceStartPoint += CrosshairWorldDirection * (DistanceToCharacter + 50.f);
		FVector LineTraceEndPoint = LineTraceStartPoint + CrosshairWorldDirection * TraceLength;

		OutEjectLocation = LineTraceStartPoint;
		OutEjectRotator = CrosshairWorldDirection.Rotation();
		
		GetWorld()->LineTraceSingleByChannel(
			LineTraceHitResult,
			LineTraceStartPoint,
			LineTraceEndPoint,
			ECC_Visibility);
		
		if (!LineTraceHitResult.bBlockingHit)
		{
			LineTraceHitResult.ImpactPoint = LineTraceEndPoint;
		}
		
		if (LineTraceHitResult.GetActor()
			&& LineTraceHitResult.GetActor()->Implements<UInteractWithEnmyInterface>())
		{
			AimDotColor = FLinearColor::Red;
		}
		else
		{
			AimDotColor = FLinearColor::White;
		}
	}
}

void APlayerCharacter::PlayMeleeAttackSound()
{
	if (MeleeAttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, MeleeAttackSound, GetActorLocation());
	}
}

void APlayerCharacter::UpdateAlertLevel(float DeltaSeconds, float InAlertLevel, bool bIsNaturalDecay)
{
	// Naturally decay until it reaches the threshold
	if (bIsNaturalDecay)
	{
		if (CurrentAlertLevel > AlertLevelNatualDecayThreshold)
		{
			CurrentAlertLevel = FMath::Max(
				AlertLevelNatualDecayThreshold,
				CurrentAlertLevel - AlertLevelDecayRate * DeltaSeconds);
		}
		return;
	}

	float NewAlertLevel = CurrentAlertLevel + InAlertLevel;

	// Check if the alert level is over the maximum
	// if so, any further increase will trigger the game over
	if (CurrentAlertLevel + 0.5f >= MaxAlertLevel)
	{
		if (InAlertLevel > 0)
		{
			if (AMainGameMode* MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				MainGameMode->HandleGameOver(false);
			}
		}
		return;
	}
	
	CurrentAlertLevel = FMath::Clamp(NewAlertLevel, 0.f, MaxAlertLevel);
}

void APlayerCharacter::UpdateSatiety(float DeltaSeconds, float InSatiety, bool bIsDecay)
{
	if (bIsDecay && !bIsInterpolatingSatiety)
	{
		Satiety =  FMath::Max(0.f, Satiety - SatietyDecayRate * DeltaSeconds);
	}
	else 
	{
		if (!bIsInterpolatingSatiety)
		{
			TargetSatiety = FMath::Clamp(Satiety + InSatiety, 0.f, MaxSatiety);
			bIsInterpolatingSatiety = true;
		}

		Satiety = FMath::FInterpTo(Satiety, TargetSatiety, DeltaSeconds, 20.f);

		if (FMath::IsNearlyEqual(Satiety, TargetSatiety, 0.1f))
		{
			bIsInterpolatingSatiety = false;
		}
	}
	
	UpdateMeshScaleAndSpringArmLocation(Satiety, MaxSatiety);
	CalcSpeedFactor(Satiety, MaxSatiety);

	if (Satiety <= 0)
	{
		if (AMainGameMode* MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			MainGameMode->HandleGameOver(false);
		}
	}
}
float APlayerCharacter::CalcSpeedFactor(float InSatiety, float InMaxSatiety)
{
	float Percentage = InSatiety / InMaxSatiety;
	
	if (Percentage <= LowerSatietyEffectThreshold)
	{
		SpeedFactor = MaxSpeedFactor;
	}
	else if (Percentage >= UpperSatietyEffectThreshold)
	{
		SpeedFactor = MinSpeedFactor;
	}
	else
	{
		float Alpha = (Percentage - LowerSatietyEffectThreshold) / (UpperSatietyEffectThreshold - LowerSatietyEffectThreshold);
		SpeedFactor = FMath::Lerp(MaxSpeedFactor, MinSpeedFactor, Alpha);
	}
	
	return SpeedFactor;
}

void APlayerCharacter::UpdateMeshScaleAndSpringArmLocation(float InSatiety, float InMaxSatiety)
{
	float Percentage = FMath::Max(InSatiety / InMaxSatiety, LowerSatietyEffectThreshold);

	GetMesh()->SetRelativeScale3D(FVector(Percentage));

	float BaseSpringArmHeight = 220.f;
	float TargetLength = 900.f * Percentage;
	FVector SpringArmLocation = SpringArmComponent->GetRelativeLocation();
	SpringArmLocation.Z = BaseSpringArmHeight * Percentage;
	SpringArmComponent->SetRelativeLocation(SpringArmLocation);
	SpringArmComponent->TargetArmLength = TargetLength;
}


void APlayerCharacter::UpdateSpeed(float InSpeed, float InSpeedFactor)
{
	GetCharacterMovement()->MaxWalkSpeed = InSpeed * InSpeedFactor;
}

void APlayerCharacter::UpdateHUD()
{
	MyPlayerController = MyPlayerController == nullptr ? Cast<AMyPlayerController>(GetController()) : MyPlayerController;
	if (MyPlayerController)
	{
		MyPlayerController->UpdateHUDAlertLevel(CurrentAlertLevel, MaxAlertLevel);
		MyPlayerController->UpdateHUDSatiety(Satiety, MaxSatiety);
		MyPlayerController->UpdateHUDRangeAttackTime(RangeAttackTimes);
		if (AimDot)
		{
			MyPlayerController->UpdateHUDAimDot(AimDot, AimDotColor);
		}
	}
}

