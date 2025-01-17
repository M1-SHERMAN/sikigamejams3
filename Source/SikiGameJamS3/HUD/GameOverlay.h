// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverlay.generated.h"

class UProgressBar;
class UTextBlock;
UCLASS()
class SIKIGAMEJAMS3_API UGameOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* AlertLevelBar;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* AlertLevelText;
	UPROPERTY(meta=(BindWidget))
	UProgressBar* SatietyBar;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SatietyText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* RangeAttackTimesText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* RemainingEnemiesText;
};
