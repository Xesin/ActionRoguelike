// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributesComponent.h"

USBTTask_RangeAttack::USBTTask_RangeAttack()
{
	MaxBulletSpread = 2.0f;
}

EBTNodeResult::Type USBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	
	if (!ensure(MyController)) return EBTNodeResult::Failed;
	ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());

	if(MyPawn == nullptr) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	if (!TargetActor) return EBTNodeResult::Failed;
	

	if (!USAttributesComponent::IsActorAlive(TargetActor))
	{
		return EBTNodeResult::Failed;
	}

	FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
	
	FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	FRotator MuzzleRotation = Direction.Rotation();	

	MuzzleRotation.Pitch += FMath::RandRange(0.f, MaxBulletSpread);
	MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Instigator = MyPawn;

	AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

	return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
