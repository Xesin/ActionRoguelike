// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseCharacter.h"
#include "AAS/SActionComponent.h"
#include "SAttributesComponent.h"

// Sets default values
ASBaseCharacter::ASBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributeComp = CreateDefaultSubobject<USAttributesComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	TimeToHitName = "TimeToHit";
}

void ASBaseCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitName, GetWorld()->TimeSeconds);
	}
}

