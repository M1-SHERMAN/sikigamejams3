// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "GameOverlay.h"
#include "Blueprint/UserWidget.h"

void AUGameHUD::BeginPlay()
{
	Super::BeginPlay();
	AddGameOverlay();
}

void AUGameHUD::AddGameOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && GameOverlayClass)
	{
		GameOverlay = CreateWidget<UGameOverlay>(PlayerController, GameOverlayClass);
		GameOverlay->AddToViewport();
	}
}
