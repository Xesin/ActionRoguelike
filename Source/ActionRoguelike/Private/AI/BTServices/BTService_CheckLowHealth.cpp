// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/BTService_CheckLowHealth.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributesComponent.h"

void UBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();

		if (!ensure(MyController)) return;

		APawn* AIPawn = MyController->GetPawn();

		if (!ensure(AIPawn)) return;

		USAttributesComponent* AttrComponent = USAttributesComponent::GetAttributes(AIPawn);

		if (!ensureMsgf(AttrComponent, TEXT("No SAttributesComponent found on pawn and it is required"))) return;

		BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, AttrComponent->GetHealth() < HealthThreshold);
	}
}
