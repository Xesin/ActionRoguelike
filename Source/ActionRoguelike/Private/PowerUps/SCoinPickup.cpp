// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUps/SCoinPickup.h"
#include "SPlayerState.h"

ASCoinPickup::ASCoinPickup()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetupAttachment(RootComponent);

	RespawnTime = 15.f;
	CoinCost = 0;
}

void ASCoinPickup::ApplyEffect(APawn* InstigatorPawn, USAttributesComponent* AttComponent)
{
	Super::ApplyEffect(InstigatorPawn, AttComponent);
	ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->AddCoins(CoinsToGive);
	}

}
