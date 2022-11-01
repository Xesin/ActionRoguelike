
#include "SAttributesComponent.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMutliplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarRageMultiplier(TEXT("su.RageGainMultiplier"), 1.0f, TEXT("Global Rage Modifier for Attribute Component."), ECVF_Cheat);

USAttributesComponent::USAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	HealthMax = 100;
	Health = HealthMax;
	Rage = 0;
	RageMax = 200;

	SetIsReplicatedByDefault(true);
}

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
	
	NetMulticastHealthChanged(InstigatorActor, Health, Delta);

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

bool USAttributesComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	if (Delta < 0.0f)
	{
		float RageMultiplier = CVarRageMultiplier.GetValueOnGameThread();
		Delta *= RageMultiplier;
	}

	float OldRage = Rage;

	Rage = FMath::Clamp(Rage += Delta, 0, RageMax);

	float ActualDelta = Rage - OldRage;
	OnRageChanged.Broadcast(InstigatorActor, this, Rage, Delta);

	NetMulticastRageChanged(InstigatorActor, Rage, Delta);

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

float USAttributesComponent::GetMaxRage() const
{
	return RageMax;
}

float USAttributesComponent::GetRage() const
{
	return Rage;
}

void USAttributesComponent::NetMulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributesComponent::NetMulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
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

void USAttributesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USAttributesComponent, Health);
	DOREPLIFETIME(USAttributesComponent, HealthMax);
	DOREPLIFETIME(USAttributesComponent, Rage);
	DOREPLIFETIME(USAttributesComponent, RageMax);
}