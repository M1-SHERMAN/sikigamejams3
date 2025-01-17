// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickable.h"
#include "Components/BoxComponent.h"

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
}

void ABasePickable::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABasePickable::OnBoxBeginOverlap);
	
}

void ABasePickable::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABasePickable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

