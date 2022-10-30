// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "PowerUps/SPowerUp.h"
#include "SHealthPotion.generated.h"
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPowerUp
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHealthPotion();

protected:
	void ApplyEffect(APawn* InstigatorPawn, USAttributesComponent* AttComponent) override;

	bool CanBeUsed(USAttributesComponent* AttComponent) override;
protected:
	UPROPERTY(VisibleAnywhere, Category = "Copmponents")
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Powerup")
	float HealthValue;
};
