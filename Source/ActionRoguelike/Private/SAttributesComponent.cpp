
#include "SAttributesComponent.h"

USAttributesComponent::USAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Health = 100;
}

bool USAttributesComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}
