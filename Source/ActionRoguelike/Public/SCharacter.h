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

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void PrimaryAttack_TimeElapsed();

	UFUNCTION()
	void SecondaryAttack_TimeElapsed();

	UFUNCTION()
	void DashAbility_TimeElapsed();
	
	UFUNCTION()
	void PrimaryAttack();

	UFUNCTION()
	void SecondaryAttack();
	
	UFUNCTION()
	void PrimaryInteract();
	
	UFUNCTION()
	void DashAbility();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta);

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> SecondaryProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* SecondaryAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<AActor> DashAbilityClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USAttributesComponent* AttributeComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* PrimaryCastVFX;
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName PrimaryCastSocket;
	
	FTimerHandle TimerHandle_MontageAnimation;
	
};
