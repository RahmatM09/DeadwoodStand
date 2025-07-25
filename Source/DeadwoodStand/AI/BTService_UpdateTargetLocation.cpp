// © 2025 Rahmat. Development Build. All rights reserved.


#include "BTService_UpdateTargetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "DeadwoodStand/Zombie/ZombieAIController.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

UBTService_UpdateTargetLocation::UBTService_UpdateTargetLocation()
{
	NodeName = "Update Target Location";
	Interval = 0.5f;
	RandomDeviation = 0.f;
}

void UBTService_UpdateTargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	UAIPerceptionComponent* PerceptionComp = AIController->FindComponentByClass<UAIPerceptionComponent>();
	if (!PerceptionComp) return;

	AActor* PlayerCharacter = nullptr;

	// Check sight
	TArray<AActor*> SightActors;
	PerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SightActors);
	for (AActor* Actor : SightActors)
	{
		if (Actor && Actor->IsA<APlayerCharacter>()) // Only track the player
		{
			PlayerCharacter = Actor;
			break;
		}
	}

	// If not seen, check hearing
	if (!PlayerCharacter)
	{
		TArray<AActor*> HeardActors;
		PerceptionComp->GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(), HeardActors);
		for (AActor* Actor : HeardActors)
		{
			if (Actor && Actor->IsA<APlayerCharacter>()) // Only track the player
			{
				PlayerCharacter = Actor;
				break;
			}
		}
	}

	// Update Blackboard
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return;

	const FName TargetActorKey = FName("TargetActor");

	if (PlayerCharacter)
	{
		Blackboard->SetValueAsObject(TargetActorKey, PlayerCharacter);
	}
	else
	{
		Blackboard->ClearValue(TargetActorKey);
	}
}
