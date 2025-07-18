// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API AProjectileWeapon : public AWeaponBase
{
	GENERATED_BODY()
public:
	AProjectileWeapon();

	virtual void Fire() override;

protected:
	/** Class of projectile to spawn (Bullet, Grenade, etc.) */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Projectile")
	TSubclassOf<AActor> ProjectileClass;

	/** Launch speed override for visual launch */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Projectile")
	float MuzzleVelocity = 2000.f;
};
