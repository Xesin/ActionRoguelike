// Fill out your copyright notice in the Description page of Project Settings.


#include "AAS/SEffect_Thorns.h"
#include "AAS/SActionComponent.h"
#include "SAttributesComponent.h"
#include "SGameplayFunctionLibrary.h"

USEffect_Thorns::USEffect_Thorns()
{
	DeflectedPercentage = 0.25f;
	Duration = 0.0f;
	Period = 0.0f;
}

void USEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	USAttributesComponent* AttributesComp = USAttributesComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributesComp)
	{
		AttributesComp->OnHealthChanged.AddDynamic(this, &USEffect_Thorns::OnHealthChanged);
	}
}

void USEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	USAttributesComponent* AttributesComp = USAttributesComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (AttributesComp)
	{
		AttributesComp->OnHealthChanged.RemoveDynamic(this, &USEffect_Thorns::OnHealthChanged);
	}
}

void USEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f && InstigatorActor != OwningComp->GetOwner())
	{
		USGameplayFunctionLibrary::ApplyDamage(GetOwningComponent()->GetOwner(), InstigatorActor, -Delta * DeflectedPercentage);
	}
}
