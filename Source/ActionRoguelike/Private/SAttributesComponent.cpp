
#include "SAttributesComponent.h"

USAttributesComponent* USAttributesComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributesComponent>(FromActor->GetComponentByClass(USAttributesComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributesComponent::IsActorAlive(AActor* FromActor)
{
	USAttributesComponent* AttributeComp = GetAttributes(FromActor);

	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

USAttributesComponent::USAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	HealthMax = 100;
}



bool USAttributesComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged()) return false;

	Health += Delta;
	Health = FMath::Clamp(Health, 0, HealthMax);
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);
	
	return true;
}

bool USAttributesComponent::IsAlive() const
{
	return Health > 0.f;
}

bool USAttributesComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

float USAttributesComponent::GetHealth() const
{
	return Health;
}

void USAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = HealthMax;
}

float USAttributesComponent::GetMaxHealth() const
{
	return HealthMax;
}
