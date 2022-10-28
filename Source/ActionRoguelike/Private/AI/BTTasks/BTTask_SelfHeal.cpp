// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_SelfHeal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributesComponent.h"

EBTNodeResult::Type UBTTask_SelfHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();

		if (!ensure(MyController)) return EBTNodeResult::Failed;

		APawn* AIPawn = MyController->GetPawn();

		if (!ensure(AIPawn)) return EBTNodeResult::Failed;

		USAttributesComponent* AttrComponent = USAttributesComponent::GetAttributes(AIPawn);

		if (!ensureMsgf(AttrComponent, TEXT("No SAttributesComponent found on pawn and it is required"))) return EBTNodeResult::Failed;

		AttrComponent->ApplyHealthChange(AIPawn, AttrComponent->GetMaxHealth());

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
