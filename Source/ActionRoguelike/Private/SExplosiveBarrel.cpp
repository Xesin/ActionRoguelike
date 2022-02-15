// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(RootComponent);
	RadialForce->ImpulseStrength = 60000.f;
	RadialForce->Radius = 350.f;

	bExploded = false;
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ASExplosiveBarrel::OnHitExplode);
}

void ASExplosiveBarrel::OnHitExplode(AActor* Actor, AActor* Actor1, FVector Vector, const FHitResult& HitResult)
{
	if(bExploded) return;

	bExploded = true;
	
	RadialForce->FireImpulse();
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

