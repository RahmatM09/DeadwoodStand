// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetRandomPatrolLocation.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API UBTTask_GetRandomPatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_GetRandomPatrolLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolRadius = 1000.f;
};
