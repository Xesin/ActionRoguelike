// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ASMagicProjectile;
class USInteractionComponent;
class USAttributesComponent;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void PrimaryAttack();

	UFUNCTION()
	void SecondaryAttack();
	
	UFUNCTION()
	void SpecialAttack();

	UFUNCTION()
	void PrimaryInteract();
	
	UFUNCTION()
	void SprintStart();

	UFUNCTION()
	void SprintStop();

	UFUNCTION()
	virtual void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributesComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName PrimaryCastSocket;
	
};
