// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AI/SAICharacter.h"
#include "EngineUtils.h"
#include "SAttributesComponent.h"

ASGameModeBase::ASGameModeBase()
{
	SpawnInterval = 2.f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_BotsSpawner, this, &ASGameModeBase::BotSpawnTimerElapsed, SpawnInterval, true);
}

void ASGameModeBase::BotSpawnTimerElapsed()
{
	int32 NumOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributesComponent* AttrComponent = USAttributesComponent::GetAttributes(Bot);

		if (ensure(AttrComponent) && AttrComponent->IsAlive())
		{
			NumOfAliveBots++;
		}
	}

	float NumMaxBots = 10;

	if (DifficultyCurve)
	{
		NumMaxBots = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}

	if (NumOfAliveBots >= NumMaxBots) return;

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, GetSpawnerQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnQueryCompleted);
	}
}

void ASGameModeBase::OnSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Get spawn locations EQS query failed!"))
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}
