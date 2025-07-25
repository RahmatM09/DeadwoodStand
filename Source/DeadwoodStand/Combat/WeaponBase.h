// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DeadwoodStand/Interactables/Pickups/Pickup.h"
#include "WeaponTypes.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class APlayerCharacter;
class UParticleSystem;
class USoundBase;
class UCameraShakeBase;
class UCrosshairWidget;
class UTexture2D;

UCLASS()
class DEADWOODSTAND_API AWeaponBase : public APickup
{
	GENERATED_BODY()

public:
	AWeaponBase();

	virtual void BeginPlay() override;
	virtual void Interact_Implementation(AActor* Interactor) override;
	
	/** Equip & attach to hand */
	virtual void Equip(APlayerCharacter* NewOwner);
	virtual void Unequip();

	/** Ammo and reload logic */
	virtual void Reload();
	bool CanReload() const;
	/** Call to start firing (e.g., input pressed) */
	virtual void StartFire();
	/** Call to stop firing (e.g., input released) */
	virtual void StopFire();
	void UpdateHUD();

	void AddAmmo(int32 Amount);
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EWeaponType WeaponType;

protected:
	/** Owner */
	UPROPERTY()
	TObjectPtr<APlayerCharacter> OwnerCharacter;

	/** Virtual Shoot (child will override) */
	virtual void Fire();
	
	/** Effects */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Effects")
	TObjectPtr<UParticleSystem> MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Effects")
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Effects")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Icons")
	TObjectPtr<UTexture2D> Icon;

	/** Stats */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Damage = 25.f;

	/** Ammo */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Ammo")
	int32 MaxAmmoInClip;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon|Ammo")
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammo")
	int32 TotalAmmo;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Reload")
	bool bIsReloading = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Reload")
	float ReloadTime = 1.5f;

	/** Reload timer */
	FTimerHandle ReloadTimerHandle;

	// Animations
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Animation")
	TObjectPtr<UAnimMontage> ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Animation")
	TObjectPtr<UAnimMontage> FireMontage;

	/** Whether weapon fires continuously when holding input */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	bool bIsAutomatic = false;

	/** Time between shots in seconds (only used if automatic) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float FireRate = 0.15f;

	/** Timer handle for auto fire */
	FTimerHandle AutoFireHandle;
	
	/** Internal complete function */
	void FinishReload();
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName AttachSocketName;

	/** Muzzle socket to spawn projectile from */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Projectile")
	FName MuzzleSocketName = "MuzzleFlash";

	bool bIsEquipped;

	/** Crosshair widget class for this weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon| UI")
	TSubclassOf<UCrosshairWidget> CrosshairWidgetClass;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;
	
public:
	/** Getters */
	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }
	FORCEINLINE int32 GetTotalAmmo() const { return TotalAmmo; }
	FORCEINLINE void SetCurrentAmmo(int32 NewCurrentAmmo) { CurrentAmmo = NewCurrentAmmo; }
	FORCEINLINE void SetTotalAmmo(int32 NewTotalAmmo) { TotalAmmo = NewTotalAmmo; }
	FORCEINLINE bool IsReloading() const { return bIsReloading; }
	TSubclassOf<class UCrosshairWidget> GetCrosshairWidgetClass() const { return CrosshairWidgetClass; }
	FORCEINLINE UTexture2D* GetWeaponIcon() const { return Icon; }
};