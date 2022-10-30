// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AAS/SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class UParticleSystem;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
	
public:
	USAction_ProjectileAttack();

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName SocketName;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;

	FTimerHandle TimerHandle_MontageAnimation;
};
