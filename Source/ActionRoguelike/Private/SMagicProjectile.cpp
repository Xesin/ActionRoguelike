// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SAttributesComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SCharacter.h"
#include "SGameplayFunctionLibrary.h"
#include "AAS/SActionComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	Damage = 5.f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USActionComponent* ActionComp = USActionComponent::GetActions(OtherActor);

		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;

			APawn* OtherPawn = Cast<APawn>(OtherActor);
			if (OtherPawn) 
			{
				SphereComp->IgnoreActorWhenMoving(GetInstigator(), false);
				SphereComp->IgnoreActorWhenMoving(OtherActor, true);
				SetInstigator(OtherPawn);
			}

			return;
		}

		USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult);

		Super::OnActorOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

