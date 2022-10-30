// Fill out your copyright notice in the Description page of Project Settings.


#include "AAS/SAction.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running; &s"), *GetNameSafe(this))
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped; &s"), *GetNameSafe(this))
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
