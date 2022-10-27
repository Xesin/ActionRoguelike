// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributesComponent.generated.h"

class USAttributesComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributesComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USAttributesComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);

	UFUNCTION(BlueprintCallable, BlueprintPure) 
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth() const;

	virtual void BeginPlay() override;
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float HealthMax;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

};
