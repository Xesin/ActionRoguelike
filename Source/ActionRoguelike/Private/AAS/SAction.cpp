// Fill out your copyright notice in the Description page of Project Settings.


#include "AAS/SAction.h"
#include "AAS/SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running; &s"), *GetNameSafe(this));

	bIsRunning = true;

	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.AppendTags(GrantTags);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped; &s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	bIsRunning = false;

	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.RemoveTags(GrantTags);
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	USActionComponent* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAnyExact(BlockedTags))
	{
		return false;
	}

	return !IsRunning();
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}

UWorld* USAction::GetWorld() const
{
	UObject* Outer = GetOuter();
	if (Outer)
	{
		return Outer->GetWorld();
	}
	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}
