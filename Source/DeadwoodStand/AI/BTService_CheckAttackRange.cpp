// © 2025 Rahmat. Development Build. All rights reserved.


#include "BTService_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
	NodeName = TEXT("Check Attack Range");
	Interval = 0.3f;
}

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return;

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(GetSelectedBlackboardKey()));
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIController ? AIController->GetPawn() : nullptr;

	if (!Target || !AIPawn) return;

	const float Distance = FVector::Dist(Target->GetActorLocation(), AIPawn->GetActorLocation());

	Blackboard->SetValueAsBool(FName("CanAttack"), Distance <= AttackRange);
}
