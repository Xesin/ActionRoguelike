// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerUps/SPowerUp.h"
#include "Components/SphereComponent.h"
#include "SAttributesComponent.h"

// Sets default values
ASPowerUp::ASPowerUp()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");

	RootComponent = SphereComp;

	RespawnTime = 10;
}

bool ASPowerUp::CanBeUsed(USAttributesComponent* AttComponent)
{
	return true;
}

void ASPowerUp::ApplyEffect(USAttributesComponent* AttComponent)
{
}

void ASPowerUp::ShowPowerup()
{
	SetPowerUpState(true);
}

void ASPowerUp::SetPowerUpState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);

	RootComponent->SetVisibility(bNewIsActive, true);
}

void ASPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	USAttributesComponent* AttrComponent = Cast<USAttributesComponent>(InstigatorPawn->GetComponentByClass(USAttributesComponent::StaticClass()));
	if (AttrComponent && CanBeUsed(AttrComponent))
	{
		ApplyEffect(AttrComponent);

		SetPowerUpState(false);

		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ASPowerUp::ShowPowerup, RespawnTime);
	}
}
