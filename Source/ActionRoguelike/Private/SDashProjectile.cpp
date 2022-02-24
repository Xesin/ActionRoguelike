// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
	MovementComp->InitialSpeed = 6000.f;
}

void ASDashProjectile::OnProjectileHit(UPrimitiveComponent* Component, AActor* Actor,
                                       UPrimitiveComponent* OtherComponent, FVector Vector, const FHitResult& HitResult)
{
	GetWorldTimerManager().ClearTimer(TimerHandle_ExplodeTimer);

	DestroyParticleAndTeleport();
}

void ASDashProjectile::TeleportInstigatorToCurrentPosition()
{
	AActor* InstigatorActor = GetInstigator();

	if(InstigatorActor)
	{
		InstigatorActor->SetActorLocation(GetActorLocation(), false, nullptr, ETeleportType::ResetPhysics);
	}
	
	Destroy();
}

void ASDashProjectile::DestroyParticleAndTeleport()
{
	MovementComp->Velocity = FVector::ZeroVector;
	EffectComp->Deactivate();
	
	if(ensure(ExplosionParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation());
	}
	
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportTimer, this, &ASDashProjectile::TeleportInstigatorToCurrentPosition, 0.2f);
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &ASDashProjectile::OnProjectileHit);
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	 GetWorldTimerManager().SetTimer(TimerHandle_ExplodeTimer, this, &ASDashProjectile::DestroyParticleAndTeleport, 0.2f);
}
