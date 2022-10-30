// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AAS/SAction.h"
#include "SAction_Effect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_Effect : public USAction
{
	GENERATED_BODY()
	
public:

	USAction_Effect();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	FTimerHandle TimerHandle_Period;
	FTimerHandle TimerHandle_Duration;
};
