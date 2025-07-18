// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "HitScanWeapon.generated.h"

class APlayerController;
/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API AHitScanWeapon : public AWeaponBase
{
	GENERATED_BODY()


public:
	AHitScanWeapon();
	virtual void Fire() override;

	
protected:
	/** Effects */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Effects")
	TObjectPtr<UParticleSystem> ImpactEffect;

	void PerformLineTrace(FHitResult& HitOut);
	void HandleImpact(const FHitResult& Hit);

	/** Stats */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Weapon")
	float FireRange = 10000.f;

};
