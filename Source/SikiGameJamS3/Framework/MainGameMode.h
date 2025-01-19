// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIKIGAMEJAMS3_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	void HandleGameOver(bool bInHasWinTheGame);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyNumberChanged, int, InRemainingEnemies);
	FOnEnemyNumberChanged OnEnemyNumberChanged;
	void OnEnemyDestroyed();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOver, bool, bHasWinTheGame);
	UPROPERTY(BlueprintAssignable, Category = "GameOver")
	FOnGameOver OnGameOver;
protected:
	virtual void BeginPlay() override;
private:
	int RemainingEnemies;
	bool bHasWinTheGame;
public:
	FORCEINLINE int GetRemainingEnemies() const { return RemainingEnemies; }
};
