// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (!TargetActor) return;

		AAIController* MyController = OwnerComp.GetAIOwner();
		if (!ensure(MyController)) return;

		APawn* AIPawn = MyController->GetPawn();

		if (!ensure(AIPawn)) return;

		float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

		bool bWithinRange = DistanceTo < 2000.f;
		bool bHasLOS = false;

		if(bWithinRange)
			bHasLOS = MyController->LineOfSightTo(TargetActor);

		BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLOS);
	}
}
