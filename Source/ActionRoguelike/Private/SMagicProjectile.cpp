// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SAttributesComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SCharacter.h"
#include "SGameplayFunctionLibrary.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	Damage = 5.f;
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult)
{
	if (Actor && Actor != GetInstigator())
	{
		USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), Actor, Damage, HitResult);

		Super::OnActorHit(PrimitiveComponent, Actor, PrimitiveComponent1, Vector, HitResult);
	}
}

