// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PerformReaperAttack.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API UBTTask_PerformReaperAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PerformReaperAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
