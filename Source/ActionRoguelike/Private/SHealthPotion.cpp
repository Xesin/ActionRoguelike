// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributesComponent.h"

// Sets default values
ASHealthPotion::ASHealthPotion()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;
	
	HealthValue = 80.f;
	RespawnTime = 5.f;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	USAttributesComponent* AttComponent = Cast<USAttributesComponent>(InstigatorPawn->GetComponentByClass(USAttributesComponent::StaticClass()));
	if (AttComponent)
	{
		AttComponent->ApplyHealthChange(HealthValue);
		
		BaseMesh->SetHiddenInGame(true, true);
		BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FTimerHandle TimerHandle_Respawn;
		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ASHealthPotion::Respawn, RespawnTime);
	}
}

void ASHealthPotion::Respawn()
{
	BaseMesh->SetHiddenInGame(false, true);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
