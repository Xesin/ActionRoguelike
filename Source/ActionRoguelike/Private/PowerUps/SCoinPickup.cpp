// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUps/SCoinPickup.h"

ASCoinPickup::ASCoinPickup()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetupAttachment(RootComponent);

	RespawnTime = 15.f;
	CoinCost = -45;
}
