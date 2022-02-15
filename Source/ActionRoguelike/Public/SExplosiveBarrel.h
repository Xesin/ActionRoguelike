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

	bool bExploded;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHitExplode(AActor* Actor, AActor* Actor1, FVector Vector, const FHitResult& HitResult);	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
