// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AI/SAICharacter.h"
#include "EngineUtils.h"
#include "SAttributesComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Storage/SSaveGame.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnInterval = 2.f;

	PlayerStateClass = ASPlayerState::StaticClass();
	SlotName = "Slot01";
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_BotsSpawner, this, &ASGameModeBase::BotSpawnTimerElapsed, SpawnInterval, true);
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);

	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delgate;
		Delgate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delgate, RespawnDelay, false);
	}
	else
	{
		Player = Cast<ASCharacter>(Killer);
		if (!Player) return;
		
		ASPlayerState* PS = Player->GetPlayerState<ASPlayerState>();
		if (!ensureMsgf(PS, TEXT("PlayerState is required to be ASPlayerState"))) return;
		
		PS->AddCoins(15);
	}
}

void ASGameModeBase::WriteSaveGame()
{
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."))
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Success load SaveGame Data."))
		}
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
	}
}

void ASGameModeBase::BotSpawnTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."))
		return;
	}

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

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributesComponent* AttrComponent = USAttributesComponent::GetAttributes(Bot);

		if (ensure(AttrComponent) && AttrComponent->IsAlive())
		{
			AttrComponent->Kill(this); // @fixme: pass in player? for kill credit
		}
	}
}
