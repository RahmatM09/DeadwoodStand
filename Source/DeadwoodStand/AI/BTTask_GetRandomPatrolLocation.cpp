// © 2025 Rahmat. Development Build. All rights reserved.


#include "BTTask_GetRandomPatrolLocation.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetRandomPatrolLocation::UBTTask_GetRandomPatrolLocation()
{
	NodeName = "Get Random Patrol Location";
}

EBTNodeResult::Type UBTTask_GetRandomPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	FNavLocation PatrolPoint;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIPawn->GetWorld());
	if (NavSys && NavSys->GetRandomPointInNavigableRadius(AIPawn->GetActorLocation(), PatrolRadius, PatrolPoint))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolLocation", PatrolPoint.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
