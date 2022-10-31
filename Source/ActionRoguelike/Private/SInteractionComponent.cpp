// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

#include "UI/SWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.2f;

	TraceDistance = 400.f;
	TraceRadius = 20.f;
	CollisionChannel = ECollisionChannel::ECC_WorldDynamic;
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());

	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void USInteractionComponent::FindBestInteractable()
{
	bool bEnableDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	UWorld* World = GetWorld();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector EyeLocation;
	FRotator EyeRotation;
	AActor* MyOwner = GetOwner();

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	float Radius = TraceRadius;

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool BlockingHit = World->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = BlockingHit ? FColor::Green : FColor::Red;

	// Clear ref before try to fill
	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();

		if (bEnableDebugDraw)
			DrawDebugSphere(World, Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f, 0.f);

		if (HitActor && HitActor->Implements<USGameplayInterface>())
		{
			FocusedActor = HitActor;
			break;
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if(!DefaultWidgetInstance->IsInViewport())
				DefaultWidgetInstance->AddToViewport();
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (bEnableDebugDraw)
		DrawDebugLine(World, EyeLocation, End, LineColor, false, 2.0f, 0.f, 2.0f);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	AActor* MyOwner = GetOwner();
	APawn* MyPawn = Cast<APawn>(MyOwner);

	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}