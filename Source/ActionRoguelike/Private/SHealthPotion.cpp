// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributesComponent.h"

// Sets default values
ASHealthPotion::ASHealthPotion()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetupAttachment(RootComponent);
	
	HealthValue = 80.f;
	RespawnTime = 5.f;
}

void ASHealthPotion::ApplyEffect(USAttributesComponent* AttComponent)
{
	AttComponent->ApplyHealthChange(HealthValue);
}

bool ASHealthPotion::CanBeUsed(USAttributesComponent* AttComponent)
{
	return AttComponent->GetHealth() < AttComponent->GetMaxHealth();
}
