// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SikiGameJamS3/Controllers/MyPlayerController.h"
#include "SikiGameJamS3/Framework/MainGameMode.h"

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

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	SetCharacterFollowCamera(false);
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateAlertLevel(DeltaSeconds, AlertLevelDecayRate, true);
	UpdateSatiety(DeltaSeconds, SatietyDecayRate,true);
	UpdateHUD();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateHUD();
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
		GetCharacterMovement()->MaxWalkSpeed = AimSpeed;
		SetCharacterFollowCamera(true);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
		GetCharacterMovement()->MaxWalkSpeed = AimSpeed;
		SetCharacterFollowCamera(true);
	}
	else
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackMontage)
		{
			AnimInstance->Montage_Play(AttackMontage);
			FName SectionName;
			float NewInAlertLevel;
			
			if (bIsAiming)
			{
				SectionName = FName("Shoot");
				NewInAlertLevel = RangeAttackAlertLevel;
				RangeAttackTimes = FMath::Max(0, RangeAttackTimes - 1);
				bIsAiming = false;
			}
			else
			{
				SectionName = FName("Melee");
				NewInAlertLevel = MeleeAttackAlertLevel;
			}
			
			UpdateAlertLevel(0.0f, NewInAlertLevel, false);
			AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
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
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
	if (CurrentAlertLevel >= MaxAlertLevel)
	{
		if (InAlertLevel > 0)
		{
			if (AMainGameMode* MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				MainGameMode->HandleGameOver();
			}
		}
		return;
	}
	
	CurrentAlertLevel = FMath::Clamp(NewAlertLevel, 0.f, MaxAlertLevel);
}

void APlayerCharacter::UpdateSatiety(float DeltaSeconds, float InHungerLevel, bool bIsNaturalDecay)
{
	if (bIsNaturalDecay)
	{
		Satiety = FMath::Max(0.f, Satiety - SatietyDecayRate * DeltaSeconds);
	}
	else
	{
		Satiety = FMath::Clamp(Satiety + InHungerLevel, 0.f, MaxSatiety);
	}
	
	if (Satiety <= 0)
	{
		if (AMainGameMode* MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			MainGameMode->HandleGameOver();
		}
	}
}

void APlayerCharacter::UpdateHUD()
{
	MyPlayerController = MyPlayerController == nullptr ? Cast<AMyPlayerController>(GetController()) : MyPlayerController;
	if (MyPlayerController)
	{
		MyPlayerController->SetHUDAlertLevel(CurrentAlertLevel, MaxAlertLevel);
		MyPlayerController->SetHUDSatiety(Satiety, MaxSatiety);
		MyPlayerController->SetHUDRangeAttackTime(RangeAttackTimes);
	}
}

