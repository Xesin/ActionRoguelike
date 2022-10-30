// Fill out your copyright notice in the Description page of Project Settings.


#include "AAS/SActionComponent.h"
#include "AAS/SAction.h"

USActionComponent::USActionComponent()
{

}

void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> Action : DefaultActions)
	{
		AddAction(Action);
	}
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass)) return;

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}

