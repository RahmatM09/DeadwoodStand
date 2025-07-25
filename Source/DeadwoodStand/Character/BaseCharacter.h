// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class DEADWOODSTAND_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WDPlayer|Stats", meta = (AllowPrivateAccess = true))
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WDPlayer|Stats", meta = (AllowPrivateAccess = true))
	float CurrentHealth;
	
	// Damage Handling
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Damage")
	void OnTakeDamage(float DamageAmount);

	virtual void TakeDamage(float DamageAmount);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UFUNCTION(BlueprintImplementableEvent, Category = "Damage")
	void OnDeath();

	// C++ event to override if needed
	virtual void HandleDeath();

public:	
	virtual void Tick(float DeltaTime) override;

};
