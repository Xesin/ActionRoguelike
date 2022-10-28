// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributesComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere, Category = "BehaviorTree")
	FName TargetActorBBKey;

	UPROPERTY(VisibleAnywhere, Category = "FX")
	FName HitFlashParamName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USAttributesComponent* AttributeComp;
};
