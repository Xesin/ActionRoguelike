
#include "SAttributesComponent.h"

USAttributesComponent::USAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	HealthMax = 100;
}



bool USAttributesComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	Health = FMath::Clamp(Health, 0, HealthMax);
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}

bool USAttributesComponent::IsAlive() const
{
	return Health > 0.f;
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
