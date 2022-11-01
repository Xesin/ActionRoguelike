// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributesComponent.generated.h"

class USAttributesComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, USAttributesComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributesComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	USAttributesComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributesComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* FromActor);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	float GetMaxRage() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attributes")
	float GetRage() const;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Attributes")
	float HealthMax;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Category = "Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Attributes")
	float RageMax;

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;

};
