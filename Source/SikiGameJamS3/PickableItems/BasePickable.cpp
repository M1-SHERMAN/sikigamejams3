// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickable.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SikiGameJamS3/Characters/PlayerCharacter.h"
#include "Sound/SoundCue.h"

ABasePickable::ABasePickable()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(CollisionBox);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ShowTipsBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ShowTipsBox"));
	ShowTipsBox->SetupAttachment(RootComponent);
	ShowTipsBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShowTipsBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	TipsTextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TipsTextWidget"));
	TipsTextWidget->SetupAttachment(ShowTipsBox);
	TipsTextWidget->SetVisibility(true);
	TipsTextWidget->SetHiddenInGame(true);
}

void ABasePickable::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABasePickable::OnCollisionBoxBeginOverlap);
	ShowTipsBox->OnComponentBeginOverlap.AddDynamic(this, &ABasePickable::OnTipsBoxBeginOverlap);
	ShowTipsBox->OnComponentEndOverlap.AddDynamic(this, &ABasePickable::OnTipsBoxEndOverlap);
	
}

void ABasePickable::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		if (PickSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickSound, GetActorLocation());
		}
	}
}

void ABasePickable::OnTipsBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor) && TipsTextWidget)
	{
		TipsTextWidget->SetHiddenInGame(false);
	}
}

void ABasePickable::OnTipsBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APlayerCharacter>(OtherActor) && TipsTextWidget)
	{
		TipsTextWidget->SetHiddenInGame(true);
	}
}

void ABasePickable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

