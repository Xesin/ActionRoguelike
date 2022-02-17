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
	RadialForce->ImpulseStrength = 2000.f;
	RadialForce->Radius = 600.f;
	RadialForce->bIgnoreOwningActor = true;
	RadialForce->bImpulseVelChange = true;
	RadialForce->AddCollisionChannelToAffect(ECC_WorldDynamic);

	MeshComp->SetSimulatePhysics(true);
	
	bExploded = false;
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnHitExplode);
}

void ASExplosiveBarrel::OnHitExplode(UPrimitiveComponent* Actor, AActor* Actor1, UPrimitiveComponent* OtherComp, FVector NormalizedImpulse, const FHitResult& HitResult)
{
	if(bExploded) return;

	bExploded = true;
	
	RadialForce->FireImpulse();

	MeshComp->SetMaterial(0, ExplodedMaterial);

	MeshComp->AddImpulse(FVector::UpVector * 1000.f, NAME_None, true);

	if(ExplosiveEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosiveEffect, GetActorLocation());
	}
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

