// Fill out your copyright notice in the Description page of Project Settings.


#include "AAS/SAction_Effect.h"
#include "AAS/SActionComponent.h"


USAction_Effect::USAction_Effect()
{
	Duration = 3;
	Period = 0.5f;
	bAutoStart = true;
}

void USAction_Effect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	
}

void USAction_Effect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Duration, Delegate, Duration, false);
	}

	if (Period > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Period, Delegate, Period, true);
	}
}

void USAction_Effect::StopAction_Implementation(AActor* Instigator)
{
	FTimerManager &TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.GetTimerRemaining(TimerHandle_Period) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	TimerManager.ClearTimer(TimerHandle_Duration);
	TimerManager.ClearTimer(TimerHandle_Period);

	USActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(Instigator, this);
	}
}
