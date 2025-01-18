// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "GameOverlay.h"
#include "Blueprint/UserWidget.h"

void AUGameHUD::DrawHUD()
{
	Super::DrawHUD();

	if (GEngine)
	{
		FVector2D ViewPortSize;
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
		const FVector2D ViewPortCenter(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
		if (AimDot)
		{
			DrawAimDot(AimDot, ViewPortCenter, AimDotColor);
		}
		
	}
}

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

void AUGameHUD::DrawAimDot(UTexture2D* Texture, const FVector2D& ViewportCenter,
	FLinearColor Color)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D DrawPosition(
		ViewportCenter.X - TextureWidth / 2.f,
		ViewportCenter.Y - TextureHeight / 2.f);
	DrawTexture(
		Texture,
		DrawPosition.X,
		DrawPosition.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		Color);
}
