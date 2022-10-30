
#include "SAttributesComponent.h"
#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMutliplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

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
	Health = HealthMax;
}



bool USAttributesComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (Delta < 0.0f)
	{
		if (!GetOwner()->CanBeDamaged()) return false;

		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;

	Health = FMath::Clamp(Health += Delta, 0, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);
	
	if (ActualDelta < 0.f && !IsAlive())
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

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
