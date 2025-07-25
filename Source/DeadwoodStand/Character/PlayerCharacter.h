// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "DeadwoodStand/Combat/WeaponTypes.h"
#include "PlayerCharacter.generated.h"

class ASaveManager;
class AWDPlayerController;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class AWeaponBase;
class ABaseHUD;
class UCrosshairWidget;
class UPlayerStatsWidget;
class UInventoryComponent;
class UPawnNoiseEmitterComponent; 

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	void UpdateWeaponSlotInfo();

	UFUNCTION()
	void PerformMeleeAttack();

	UFUNCTION()
	void ResetMeleeAttackState();

	UFUNCTION(BlueprintCallable)
	TArray<FWeaponData> CollectWeaponData() const;
	void RestoreWeapons(const TArray<FWeaponData>& SavedWeapons);


	// Health and Stamina Section
	int32 HealthArray = 2;
	int32 StaminaArray = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsMeleeAttacking = false;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MeleeDamage = 100.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MeleeRange = 100.f;

	FTimerHandle MeleeResetHandle;


	UFUNCTION()
	void OnNightStateChanged(bool bIsNight);

	void SaveGameProgress();
	void LoadGameProgress();

	UPROPERTY()
	TObjectPtr<ASaveManager> SaveManager = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TMap<EWeaponType, TSubclassOf<AWeaponBase>> WeaponClassMap;
	
	UFUNCTION(BlueprintCallable)
	void MakeMovementNoise();

	/** Movement input handlers */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump();
	void StopJump(const FInputActionValue& Value);
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);
	void ToggleCrouch(const FInputActionValue& Value);
	void HandleInteract();
	void Fire();
	void StopFire();
	void Reload();
	void StartAim();
	void StopAim();
	void EquipWeaponSlot1();
	void EquipWeaponSlot2();
	void MeleeAttack();
	void UseHealthPickup();
	void UseStaminaPickup();


	/* Weapon Section ***********************************************************/
	void EquipWeaponFromSlot(TObjectPtr<AWeaponBase>& NewWeapon);

	
	/* Camera Section ***********************************************************/
	void SetAimingCameraFOV(float DeltaTime);

	
	/* Input Actions Section ***********************************************************/
	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> ReloadAction;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> WeaponSlot1Action;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> WeaponSlot2Action;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> MeleeAttackAction;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> UseHealthAction;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Input")
	TObjectPtr<UInputAction> UseStaminaAction;

	/* Components Section ***********************************************************/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WDPlayer|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WDPlayer|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UPawnNoiseEmitterComponent> NoiseEmitter;

private:
	// Controller
	UPROPERTY()
	TObjectPtr<AWDPlayerController> WDPlayerController;

	UPROPERTY()
	TObjectPtr<ABaseHUD> BaseHUD;

	
	// Walk and sprint Speed
	float WalkSpeed;
	float SprintSpeed;
	bool bIsSprinting;

	/* Health and Stamina Section ***********************************************************/
	UPROPERTY(EditAnywhere, Category = "WDPlayer|Stamina")
	float MaxStamina = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "WDPlayer|Stamina")
	float CurrentStamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "WDPlayer|Stamina")
	float StaminaDrainRate = 25.f; // per second

	UPROPERTY(EditAnywhere, Category = "WDPlayer|Stamina")
	float StaminaRecoveryRate = 15.f; // per second

	// Health and Stamina properties

	FTimerHandle StaminaRecoveryTimerHandle;
	bool bCanRecoverStamina = true;
	void EnableStaminaRecovery();
	
	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Stamina")
	float StaminaRecoveryDelay = 10.0f;
	
	/* Damage Section ***********************************************************/
	virtual void TakeDamage(float DamageAmount) override;
	virtual void HandleDeath() override;

	/* HUD Section ***********************************************************/
	UPROPERTY()
	TObjectPtr<UPlayerStatsWidget> StatsWidget;

	void UpdateCrosshairSpread(float DeltaTime);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WDPlayer|Crosshairs", meta = (AllowPrivateAccess = true))
	TSubclassOf<UCrosshairWidget> DefautlCrosshairWidgetClass;
	
	/* Weapon Section ***********************************************************/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WDPlayer|Weapon", meta = (AllowPrivateAccess = true))
	TObjectPtr<AWeaponBase> CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WDPlayer|Weapon", meta = (AllowPrivateAccess = true))
	EWeaponType CurrentWeaponType;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Aiming")
	float DefaultFOV = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Aiming")
	float AimedFOV = 65.f;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Aiming")
	float AimInterpSpeed = 10.f;

	bool bIsAiming = false;
	float CurrentFOV = 90.f;
	bool bIsShooting = false;
	FTimerHandle ShootingTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "WDPlayer|Crosshair")
	float ShootingSpreadDuration = 0.2f;
	
	void StopShootingEffect();
	float CurrentSpread;

	UPROPERTY(EditAnywhere, Category = "WDPlayer|Combat")
	UAnimMontage* MeleeAttackMontage;

	int32 DaysSurvived = 0;

	
	friend class UInventoryComponent;
	
	/* Helper Functions  */
	void InitializeInput();
	void InitializeHUD();
	void RefreshHUDForWeapon();
	void UpdateStamina(float DeltaTime);
	
public:
	/** Setters & Getters */
	FORCEINLINE AWDPlayerController* GetWDPlayerController() const {return WDPlayerController;}
	FORCEINLINE float GetSpreadAmount() const { return CurrentSpread; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE float GetCurrentStamina() const { return CurrentStamina; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	FORCEINLINE int32 GetHealthArray() const { return HealthArray; }
	FORCEINLINE int32 GetStaminaArray() const { return StaminaArray; }
	FORCEINLINE int32 GetDaysSurvived() const { return DaysSurvived; }
	FORCEINLINE AWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

	void SetCurrentHealth(float NewHealth);
	FORCEINLINE void SetCurrentStamina(float NewStamina) { CurrentStamina = NewStamina; }
	FORCEINLINE void SetDaysSurvived(int32 NewDaysSurvived) { DaysSurvived = NewDaysSurvived; }
	FORCEINLINE void ResetHealthArray(int32 NewHealthArray) { HealthArray = NewHealthArray; }
	FORCEINLINE void ResetStaminaArray(int32 NewStaminaArray) { StaminaArray = NewStaminaArray; }
	FORCEINLINE void SetCurrentWeapon(AWeaponBase* NewWeapon);
	
	void SetHealthArray(int32 NewHealthArray);
	void SetStaminaArray(int32 NewStaminaArray);
	void StartShootingEffect();
	
};
