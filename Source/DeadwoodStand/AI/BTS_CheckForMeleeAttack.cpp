// © 2025 Rahmat. Development Build. All rights reserved.


#include "BTS_CheckForMeleeAttack.h"

#include "DeadwoodStand/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_CheckForMeleeAttack::UBTS_CheckForMeleeAttack()
{
	NodeName = "Check For Melee Attack";
}

void UBTS_CheckForMeleeAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;
	BB->SetValueAsBool("PlayerIsMeleeAttacking", Player->bIsMeleeAttacking);
}
