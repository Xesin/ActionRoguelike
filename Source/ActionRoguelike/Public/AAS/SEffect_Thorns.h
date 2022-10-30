// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AAS/SAction_Effect.h"
#include "SEffect_Thorns.generated.h"

class USAttributesComponent;

UCLASS()
class ACTIONROGUELIKE_API USEffect_Thorns : public USAction_Effect
{
	GENERATED_BODY()
	
public:
	USEffect_Thorns();

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeflectedPercentage;
};
