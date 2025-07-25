// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TTask_ScreamerScreech.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API UTTask_ScreamerScreech : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTTask_ScreamerScreech();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
