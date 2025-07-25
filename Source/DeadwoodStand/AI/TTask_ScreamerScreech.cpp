// © 2025 Rahmat. Development Build. All rights reserved.


#include "TTask_ScreamerScreech.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeadwoodStand/Zombie/ZombieBaseCharacter.h"

UTTask_ScreamerScreech::UTTask_ScreamerScreech()
{
	NodeName = "Screamer Screech";
}

EBTNodeResult::Type UTTask_ScreamerScreech::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	//if (Blackboard->GetValueAsBool("Screamed"))
	//	return EBTNodeResult::Succeeded;

	AZombieBaseCharacter* Zombie = Cast<AZombieBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Zombie) return EBTNodeResult::Failed;

	// Play screech animation/sound
	Zombie->PlayScreechFX(); // Implement this in the zombie class

	//Blackboard->SetValueAsBool("Screamed", true);

	return EBTNodeResult::Succeeded;
	
}
