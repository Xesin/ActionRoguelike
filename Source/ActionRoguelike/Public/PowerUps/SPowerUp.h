// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPowerUp.generated.h"

class USphereComponent;
class USAttributesComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPowerUp : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerUp();
	
protected:
	virtual bool CanBeUsed(USAttributesComponent* AttComponent);
	
	virtual void ApplyEffect(APawn* InstigatorPawn, USAttributesComponent* AttComponent);

	UFUNCTION(NetMulticast, Unreliable)
	virtual void NetMulticast_ExecuteUsageVFX(APawn* InstigatorPawn);

	void ShowPowerup();

	void SetPowerUpState(bool bNewIsActive);

	void Interact_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION()
	void OnRep_IsVisible();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	float RespawnTime;

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	int32 CoinCost;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(ReplicatedUsing = OnRep_IsVisible)
	bool bIsVisible;

	FTimerHandle TimerHandle_Respawn;
};
