// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere);
	URadialForceComponent* RadialForce;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* ExplodedMaterial;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ExplosiveEffect;

	bool bExploded;
	
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHitExplode(UPrimitiveComponent* Actor, AActor* Actor1, UPrimitiveComponent* OtherComp, FVector NormalizedImpulse, const FHitResult& HitResult);	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
