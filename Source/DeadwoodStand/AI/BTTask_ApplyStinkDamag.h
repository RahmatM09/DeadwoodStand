// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ApplyStinkDamag.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API UBTTask_ApplyStinkDamag : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_ApplyStinkDamag();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	float Radius = 300.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float DamageAmount = 3.f;
};
