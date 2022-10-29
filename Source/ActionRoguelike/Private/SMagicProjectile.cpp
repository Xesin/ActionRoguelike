// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SAttributesComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SCharacter.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	Damage = 5.f;
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult)
{
	if (Actor && Actor != GetInstigator())
	{
		USAttributesComponent* Attributes = USAttributesComponent::GetAttributes(Actor);
		if (Attributes)
		{
			Attributes->ApplyHealthChange(GetInstigator(), - Damage);

		}
		Super::OnActorHit(PrimitiveComponent, Actor, PrimitiveComponent1, Vector, HitResult);
	}
}

