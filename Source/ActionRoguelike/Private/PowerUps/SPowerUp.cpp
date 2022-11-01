// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerUps/SPowerUp.h"
#include "Components/SphereComponent.h"
#include "SAttributesComponent.h"
#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerUp::ASPowerUp()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");

	RootComponent = SphereComp;

	RespawnTime = 10;
	CoinCost = 0;
	bIsVisible = true;
	SetReplicates(true);
}

bool ASPowerUp::CanBeUsed(USAttributesComponent* AttComponent)
{
	return true;
}

void ASPowerUp::ApplyEffect(APawn* InstigatorPawn, USAttributesComponent* AttComponent)
{
}

void ASPowerUp::NetMulticast_ExecuteUsageVFX_Implementation(APawn* InstigatorPawn)
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
	bIsVisible = bNewIsActive;
}

void ASPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	USAttributesComponent* AttrComponent = USAttributesComponent::GetAttributes(InstigatorPawn);
	if (AttrComponent && CanBeUsed(AttrComponent))
	{
		ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>();
		if (PS)
		{
			if (!PS->RemoveCoins(CoinCost)) return;
		}

		ApplyEffect(InstigatorPawn, AttrComponent);

		NetMulticast_ExecuteUsageVFX(InstigatorPawn);

		SetPowerUpState(false);

		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ASPowerUp::ShowPowerup, RespawnTime);
	}
}

void ASPowerUp::OnRep_IsVisible()
{
	if (bIsVisible)
	{
		SetPowerUpState(true);
	}
	else
	{
		SetPowerUpState(false);
	}
}

void ASPowerUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPowerUp, bIsVisible);
}