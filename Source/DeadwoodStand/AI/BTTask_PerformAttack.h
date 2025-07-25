// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PerformAttack.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API UBTTask_PerformAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PerformAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float DamageAmount = 5.f;
};
