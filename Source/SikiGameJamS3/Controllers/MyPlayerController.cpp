// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "SikiGameJamS3/Characters/PlayerCharacter.h"
#include "SikiGameJamS3/HUD/GameHUD.h"
#include "SikiGameJamS3/HUD/GameOverlay.h"


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TryGetPlayerCharacter();
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			EnhancedInputComponent->BindAction(Movement,ETriggerEvent::Triggered , this, &ThisClass::OnMovementTriggered);
			EnhancedInputComponent->BindAction(Look,ETriggerEvent::Triggered , this, &ThisClass::OnLookTriggered);
			EnhancedInputComponent->BindAction(Aim,ETriggerEvent::Triggered , this, &ThisClass::OnAimTriggered);
			EnhancedInputComponent->BindAction(Aim,ETriggerEvent::Completed , this, &ThisClass::OnAimTriggered);
			EnhancedInputComponent->BindAction(Attack,ETriggerEvent::Triggered , this, &ThisClass::OnAttackTriggered);
			EnhancedInputComponent->BindAction(Attack,ETriggerEvent::Completed , this, &ThisClass::OnAttackTriggered);
			EnhancedInputComponent->BindAction(Interact,ETriggerEvent::Started , this, &ThisClass::OnInteractStarted);
			EnhancedInputComponent->BindAction(Jump,ETriggerEvent::Started , this, &ThisClass::OnJumpStarted);
			EnhancedInputComponent->BindAction(Sprint,ETriggerEvent::Triggered , this, &ThisClass::OnSprintTriggered);
			EnhancedInputComponent->BindAction(Sprint,ETriggerEvent::Completed , this, &ThisClass::OnSprintCompleted);
		}
	}
}

void AMyPlayerController::TryGetPlayerCharacter()
{
	OwningPlayerCharacter = OwningPlayerCharacter == nullptr ? Cast<APlayerCharacter>(GetPawn()) : OwningPlayerCharacter;
}

void AMyPlayerController::SetHUDAlertLevel(float AlertLevel, float MaxAlertLevel)
{
	TryGetPlayerCharacter();
	GameHUD = GameHUD == nullptr ? Cast<AUGameHUD>(GetHUD()) : GameHUD;

	if (OwningPlayerCharacter
		&& GameHUD
		&& GameHUD->GameOverlay
		&& GameHUD->GameOverlay->AlertLevelText
		&& GameHUD->GameOverlay->AlertLevelBar)
	{
		GameHUD->GameOverlay->AlertLevelBar->SetPercent(AlertLevel / MaxAlertLevel);
		FString AlertLevelString = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(AlertLevel), FMath::CeilToInt(MaxAlertLevel));
		GameHUD->GameOverlay->AlertLevelText->SetText(FText::FromString(AlertLevelString));
	}
}

void AMyPlayerController::SetHUDSatiety(float Satiety, float MaxSatiety)
{
	TryGetPlayerCharacter();
	GameHUD = GameHUD == nullptr ? Cast<AUGameHUD>(GetHUD()) : GameHUD;

	if (OwningPlayerCharacter
		&& GameHUD
		&& GameHUD->GameOverlay
		&& GameHUD->GameOverlay->SatietyBar
		&& GameHUD->GameOverlay->SatietyText)
	{
		GameHUD->GameOverlay->SatietyBar->SetPercent(Satiety / MaxSatiety);
		FString AlertLevelString = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Satiety), FMath::CeilToInt(MaxSatiety));
		GameHUD->GameOverlay->SatietyText->SetText(FText::FromString(AlertLevelString));
	}
}

void AMyPlayerController::SetHUDRangeAttackTime(int InRangeAttackTime)
{
	TryGetPlayerCharacter();
	GameHUD = GameHUD == nullptr ? Cast<AUGameHUD>(GetHUD()) : GameHUD;
	if (OwningPlayerCharacter
		&& GameHUD
		&& GameHUD->GameOverlay
		&& GameHUD->GameOverlay->RangeAttackTimesText)
	{
		GameHUD->GameOverlay->RangeAttackTimesText->SetText(FText::FromString(FString::Printf(TEXT("%d"), InRangeAttackTime)));
	}
}

void AMyPlayerController::SetHUDRemainingEnemies(int InRemainingEnemies)
{
}

void AMyPlayerController::OnMovementTriggered(const FInputActionValue& Value)
{
	TryGetPlayerCharacter();
	OwningPlayerCharacter->HandleMovement(Value.Get<FVector2d>());
}

void AMyPlayerController::OnLookTriggered(const FInputActionValue& Value)
{
	TryGetPlayerCharacter();
	OwningPlayerCharacter->HandleLook(Value.Get<FVector2d>());
}

void AMyPlayerController::OnAimTriggered(const FInputActionValue& Value)
{
	TryGetPlayerCharacter();
	OwningPlayerCharacter->HandleAim(Value.Get<bool>());
}

void AMyPlayerController::OnAimCompleted(const FInputActionValue& Value)
{
	TryGetPlayerCharacter();
	OwningPlayerCharacter->HandleAim(Value.Get<bool>());
}

void AMyPlayerController::OnAttackTriggered(const FInputActionValue& Value)
{
	TryGetPlayerCharacter();
	OwningPlayerCharacter->HandleAttack(Value.Get<bool>());
}

void AMyPlayerController::OnAttackCompleted(const FInputActionValue& Value)
{
	TryGetPlayerCharacter();
	OwningPlayerCharacter->HandleAttack(Value.Get<bool>());
}

void AMyPlayerController::OnInteractStarted(const FInputActionValue& Value)
{
	TryGetPlayerCharacter();
	OwningPlayerCharacter->HandleInteract(true);
}

void AMyPlayerController::OnJumpStarted(const FInputActionValue& Value)
{
	TryGetPlayerCharacter();
	OwningPlayerCharacter->HandleJump();
}

void AMyPlayerController::OnSprintTriggered(const FInputActionValue& Value)
{
	TryGetPlayerCharacter();
	OwningPlayerCharacter->HandleSprint(Value.Get<bool>());
}

void AMyPlayerController::OnSprintCompleted(const FInputActionValue& Value)
{
	TryGetPlayerCharacter();
	OwningPlayerCharacter->HandleSprint(Value.Get<bool>());
}
