// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckForMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API UBTS_CheckForMeleeAttack : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_CheckForMeleeAttack();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
