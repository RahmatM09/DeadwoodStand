// © 2025 Rahmat. Development Build. All rights reserved.


#include "BTTask_PerformReaperAttack.h"

#include "AIController.h"
#include "DeadwoodStand/Zombie/ReaperZombieCharacter.h"

UBTTask_PerformReaperAttack::UBTTask_PerformReaperAttack()
{
	NodeName = "Perform Reaper Attack";
}

EBTNodeResult::Type UBTTask_PerformReaperAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AReaperZombieCharacter* Reaper = Cast<AReaperZombieCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Reaper) return EBTNodeResult::Failed;

	Reaper->PerformReaperAttack();
	return EBTNodeResult::Succeeded;
}

