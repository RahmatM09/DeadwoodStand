// © 2025 Rahmat. Development Build. All rights reserved.


#include "MyBTTask_Doge.h"

#include "AIController.h"
#include "DeadwoodStand/Zombie/ReaperZombieCharacter.h"


UMyBTTask_Doge::UMyBTTask_Doge()
{
	NodeName = "Zombie Doge";
}

EBTNodeResult::Type UMyBTTask_Doge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AReaperZombieCharacter* Reaper = Cast<AReaperZombieCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Reaper) return EBTNodeResult::Failed;

	Reaper->TryDodge();
	return EBTNodeResult::Succeeded;
}
