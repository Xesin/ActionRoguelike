// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUps/SPowerUp.h"
#include "SCoinPickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASCoinPickup : public ASPowerUp
{
	GENERATED_BODY()
	
public:
	ASCoinPickup();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Copmponents")
	UStaticMeshComponent* BaseMesh;
};
