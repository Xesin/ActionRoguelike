// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SAttributesComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int I,
	bool bArg, const FHitResult& HitResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		USAttributesComponent* Attributes = Cast<USAttributesComponent>(OtherActor->GetComponentByClass(USAttributesComponent::StaticClass()));
		if(Attributes)
		{
			Attributes->ApplyHealthChange(-20.f);

			Destroy();
		}
	}
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

