// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "SikiGameJamS3/Characters/EnemyCharacter.h"



void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AEnemyCharacter::StaticClass(),
		FoundEnemies);
	RemainingEnemies = FoundEnemies.Num();
	OnEnemyNumberChanged.Broadcast(RemainingEnemies);
}

void AMainGameMode::HandleGameOver(bool bHasWinTheGame)
{
	if (bHasWinTheGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("You Win!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("You Lose!"));
	}
}

void AMainGameMode::OnEnemyDestroyed()
{
	RemainingEnemies--;
	OnEnemyNumberChanged.Broadcast(RemainingEnemies);
	if (RemainingEnemies <= 0)
	{
		HandleGameOver(true);
	}
}


