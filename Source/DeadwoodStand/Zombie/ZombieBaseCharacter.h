// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DeadwoodStand/Character/BaseCharacter.h"
#include "ZombieType.h"
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

	// --- Zombie Type ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zombie")
	EZombieType ZombieType;

	UFUNCTION(BlueprintCallable, Category = "Zombie")
	EZombieType GetZombieType() const { return ZombieType; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// === Combat ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zombie|Combat")
	float AttackRange = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zombie|Combat")
	float AttackDamage = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zombie|Combat")
	float AttackCooldown = 2.f;

	UPROPERTY(BlueprintReadOnly, Category = "Zombie|Combat")
	bool bCanAttack = true;

	// === Movement ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zombie|Movement")
	float MovementSpeed = 300.f;

	// === Target ===
	UPROPERTY(BlueprintReadOnly, Category = "Zombie|Target")
	TObjectPtr<APlayerCharacter> TargetPlayer;

	//  === Sounds ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zombie|Sounds")
	TObjectPtr<USoundBase> ScreechSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zombie|Sounds")
	float ScreamVolume = 1.f;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zombie|Combat")
	TObjectPtr<UAnimMontage> AttackMontage;

	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE void SetCurrentHealth(float NewHealth)  { CurrentHealth = NewHealth; }

	void PlayScreechFX();
};
