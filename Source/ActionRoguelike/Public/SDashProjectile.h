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
	void OnProjectileHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult);

	UFUNCTION()
	void TeleportInstigatorToCurrentPosition();

	UFUNCTION(BlueprintCallable)
	void DestroyParticleAndTeleport();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
protected:
	FTimerHandle TimerHandle_ExplodeTimer;
	FTimerHandle TimerHandle_TeleportTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* ExplosionParticle;
};
