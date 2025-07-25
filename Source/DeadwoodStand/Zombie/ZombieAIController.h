// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"


class UBehaviorTree;
class UBlackboardComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

public:
	AZombieAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	/** Blackboard access */
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return Blackboard; }

	APawn* GetTargetPawn() const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	// Blackboard & BehaviorTree
	UPROPERTY(Transient)
	TObjectPtr<UBehaviorTreeComponent> BehaviorComp;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	FName bHasLineOfSightKey = "bHasLineOfSight";

	/** Perception updates */
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
