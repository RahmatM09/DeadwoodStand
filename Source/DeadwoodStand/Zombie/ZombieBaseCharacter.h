// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DeadwoodStand/Character/BaseCharacter.h"
#include "ZombieBaseCharacter.generated.h"

class APlayerCharacter;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API AZombieBaseCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AZombieBaseCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// === Combat ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float AttackRange = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float AttackDamage = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float AttackCooldown = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bCanAttack = true;

	// === Movement ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed = 300.f;

	// === Target ===
	UPROPERTY(BlueprintReadOnly, Category = "Target")
	TObjectPtr<APlayerCharacter> TargetPlayer;
};
