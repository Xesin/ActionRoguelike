// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASBaseProjectile::ASBaseProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASBaseProjectile::OnActorOverlap);
	
	RootComponent = SphereComp;	
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.f;
	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

	InitialLifeSpan = 0.5f;
	ShakeInnerRadius = 50.f;
	ShakeOuterRadius = 300.f;
	ShakeFalloff = 1;

	//SetReplicates(true);
}

void ASBaseProjectile::Explode_Implementation()
{
	if (ensure(!IsPendingKill()))
	{
		if(ImpactVFX)
			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		if(ImpactSFX)
			UGameplayStatics::SpawnSoundAtLocation(this, ImpactSFX, GetActorLocation(), GetActorRotation());
		
		if (ImpactCameraShake)
			UGameplayStatics::PlayWorldCameraShake(this, ImpactCameraShake, GetActorLocation(), ShakeInnerRadius, ShakeOuterRadius, ShakeFalloff, true);

		Destroy();
	}
}

void ASBaseProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Explode();
}

void ASBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ASBaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
