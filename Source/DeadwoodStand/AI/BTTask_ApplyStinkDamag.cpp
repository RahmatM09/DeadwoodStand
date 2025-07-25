// © 2025 Rahmat. Development Build. All rights reserved.


#include "BTTask_ApplyStinkDamag.h"

#include "AIController.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"
#include "DeadwoodStand/Zombie/ZombieBaseCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTTask_ApplyStinkDamag::UBTTask_ApplyStinkDamag()
{
	NodeName = "Stink Damage AoE";
}

EBTNodeResult::Type UBTTask_ApplyStinkDamag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AZombieBaseCharacter* Zombie = Cast<AZombieBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Zombie) return EBTNodeResult::Failed;

	TArray<AActor*> OverlappingActors;
	UGameplayStatics::GetAllActorsOfClass(Zombie->GetWorld(), APlayerCharacter::StaticClass(), OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (FVector::Dist(Actor->GetActorLocation(), Zombie->GetActorLocation()) <= Radius)
		{
			UGameplayStatics::ApplyPointDamage(Actor, DamageAmount, Zombie->GetActorForwardVector(), FHitResult(), Zombie->GetController(), Zombie, nullptr);
		}
	}

	return EBTNodeResult::Succeeded;
	
}


