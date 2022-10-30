// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseCharacter.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributesComponent;
class USWorldUserWidget;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ASBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta) override;

	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditAnywhere, Category = "BehaviorTree")
	FName TargetActorBBKey;

	USWorldUserWidget* ActiveHealthBar;
};
