// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API UBTService_CheckAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CheckAttackRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 150.f;
};
