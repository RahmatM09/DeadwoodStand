// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsWidget.generated.h"

class UProgressBar;
class APlayerCharacter;

// Forward Declaration
class UTexture2D;

USTRUCT(BlueprintType)
struct FWeaponSlotInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CurrentAmmo = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TotalAmmo = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bNotEmpty = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture2D> Icon = nullptr;
};

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API UPlayerStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeStats(APlayerCharacter* InCharacter);

	void UpdateHealthBar(float Percent);
	void UpdateStaminaBar(float Percent);

	// New event to notify HealthArray count change
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnHealthArrayChanged(int32 NewCount);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnStaminaArrayChanged(int32 NewCount);

	/** Update weapon ammo (Current / Total) - Blueprint Native Event */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void UpdateAmmoInfo(int32 CurrentAmmo, int32 TotalAmmo);
	virtual void UpdateAmmoInfo_Implementation(int32 CurrentAmmo, int32 TotalAmmo);

	// These slots store the weapon info to be displayed on HUD
	UPROPERTY(BlueprintReadOnly, Category = "Weapons")
	FWeaponSlotInfo WeaponSlot1;

	UPROPERTY(BlueprintReadOnly, Category = "Weapons")
	FWeaponSlotInfo WeaponSlot2;

	UPROPERTY(BlueprintReadOnly, Category = "Weapons")
	FWeaponSlotInfo WeaponSlot3;

	// Called from C++ when weapon info is updated (ammo change, new weapon, etc.)
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapons")
	void OnWeaponSlotsUpdated();
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	float HealthValue = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	float StaminaValue = 1.f;

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerRef;
};
