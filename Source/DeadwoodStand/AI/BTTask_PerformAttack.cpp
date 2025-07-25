// © 2025 Rahmat. Development Build. All rights reserved.


#include "BTTask_PerformAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"
#include "DeadwoodStand/Zombie/ZombieBaseCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTTask_PerformAttack::UBTTask_PerformAttack()
{
	NodeName = "Perform Zombie Attack";
}

EBTNodeResult::Type UBTTask_PerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	AZombieBaseCharacter* Zombie = Cast<AZombieBaseCharacter>(AIController->GetPawn());
	if (!Zombie) return EBTNodeResult::Failed;
	
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	APlayerCharacter* Player = Cast<APlayerCharacter>(TargetActor);
	if (!Player) return EBTNodeResult::Failed;
	
	// Trigger animation (optional)
	if (Zombie->AttackMontage)
	{
		Zombie->PlayAnimMontage(Zombie->AttackMontage);
	}

	// Define required parameters
	FVector HitFromDirection = (Player->GetActorLocation() - Zombie->GetActorLocation()).GetSafeNormal();
	FHitResult DummyHitResult; // Not used in this context, but needed for API

	UGameplayStatics::ApplyPointDamage(
		Player,                         // Damaged Actor
		DamageAmount,                   // Base Damage
		HitFromDirection,              // Direction
		DummyHitResult,                // Optional HitResult (use if needed)
		Zombie->GetController(),       // InstigatedBy
		Zombie,                        // DamageCauser
		UDamageType::StaticClass()     // DamageTypeClass
	);

	return EBTNodeResult::Succeeded;
}
