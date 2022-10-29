// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

void USInteractionComponent::PrimaryInteract()
{
	bool bEnableDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	UWorld* World = GetWorld();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	

	FVector EyeLocation;
	FRotator EyeRotation;
	AActor* MyOwner = GetOwner();

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + (EyeRotation.Vector() * 500);
	
	// FHitResult Hit;
	// bool BlockingHit = World->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	float Radius = 30.f;
	
	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	
	bool BlockingHit = World->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = BlockingHit ? FColor::Green : FColor::Red;
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();

		if(bEnableDebugDraw)
			DrawDebugSphere(World, Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f, 0.f);

		if(HitActor && HitActor->Implements<USGameplayInterface>())
		{
			APawn* MyPawn = Cast<APawn>(MyOwner);
			
			ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
			break;
		}
	}
	if(bEnableDebugDraw)
		DrawDebugLine(World, EyeLocation, End, LineColor, false, 2.0f, 0.f, 2.0f);
}
