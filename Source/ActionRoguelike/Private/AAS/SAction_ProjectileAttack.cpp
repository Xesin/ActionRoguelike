// Fill out your copyright notice in the Description page of Project Settings.


#include "AAS/SAction_ProjectileAttack.h"
#include "SBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	SocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (TimerHandle_MontageAnimation.IsValid()) return;


	ASBaseCharacter* Character = Cast<ASBaseCharacter>(Instigator);

	if (!Character) return;

	if (ensure(AttackAnim))
		Character->PlayAnimMontage(AttackAnim);

	if (ensure(CastingEffect))
	{
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), SocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
	}

	FTimerHandle TimerHandle_AttackDelay;

	FTimerDelegate Delgate;
	Delgate.BindUFunction(this, "AttackDelay_Elapsed", Character);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delgate, AttackAnimDelay, false);
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensure(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(SocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;


		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		FVector RayDirection = InstigatorCharacter->GetControlRotation().Vector();
		FVector TraceEnd = TraceStart + (RayDirection * 5000);

		FHitResult Hit;

		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}
