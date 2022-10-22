// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "SDashProjectile.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASBaseProjectile
{
	GENERATED_BODY()

public:
	ASDashProjectile();
	
protected:
	UFUNCTION()
	void TeleportInstigator();

	virtual void Explode_Implementation() override;

	virtual void BeginPlay() override;
protected:
	FTimerHandle TimerHandle_DelayedDetonate;

	UPROPERTY(EditDefaultsOnly, Category = "Telepor")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Telepor")
	float DetonateDelay;
};
