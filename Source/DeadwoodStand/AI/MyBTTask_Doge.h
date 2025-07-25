// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTask_Doge.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API UMyBTTask_Doge : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UMyBTTask_Doge();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
