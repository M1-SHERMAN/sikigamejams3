// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SikiGameJamS3/Characters/EnemyCharacter.h"
#include "SikiGameJamS3/Interfaces/InteractWithEnmyInterface.h"
#include "Sound/SoundCue.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (TraceParticle)
	{
		ParticleSystemComponent = UGameplayStatics::SpawnEmitterAttached(
			TraceParticle,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition);
	}
	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
	
	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	UE_LOG(LogTemp, Warning, TEXT("Projectile Spawned At Location: %s"), *GetActorLocation().ToString());
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor
		&& OtherActor != this
		&& OtherActor != GetOwner()
		&& OtherActor->Implements<UInteractWithEnmyInterface>())
	{
		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
		}

		if (ImpactParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticle, GetActorLocation());
		}
		
		if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor))
		{
			EnemyCharacter->Execute_GettingAttack(OtherActor);
		}
	}
	
	Destroy();
		
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

