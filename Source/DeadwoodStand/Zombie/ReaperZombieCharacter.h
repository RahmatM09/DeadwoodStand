// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ZombieBaseCharacter.h"
#include "ReaperZombieCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API AReaperZombieCharacter : public AZombieBaseCharacter
{
	GENERATED_BODY()
public:
	AReaperZombieCharacter();

	// Attack logic
	UFUNCTION()
	void PerformReaperAttack();

	UFUNCTION()
	void TryDodge();

	bool bIsDodging = false;
	FTimerHandle DodgeCooldownHandle;

protected:
	virtual void BeginPlay() override;

	void StopDodging();
	// Timer to delay attacks
	FTimerHandle AttackCooldownHandle;

	UPROPERTY(EditAnywhere, Category = "Zombie|Reaper|Combat")
	float MeleeAttackRange = 150.f;

	UPROPERTY(EditAnywhere, Category = "Zombie|Reaper|Combat")
	float MeleeAttackDamage = 40.f;

	UPROPERTY(EditAnywhere, Category = "Zombie|Reaper|Combat")
	UAnimMontage* DodgeMontage;

	bool bIsAttacking;
};
