// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeadwoodStand/Combat/WeaponBase.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEADWOODSTAND_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	
	// Adds a weapon to the inventory
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddWeapon(AWeaponBase* NewWeapon);

	// Swaps the current weapon
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SwapWeapon();

	UPROPERTY(BlueprintReadOnly)
	bool bWeaponsSlotFull = false;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<AWeaponBase> WeaponSlot1;

	UPROPERTY()
	TObjectPtr<AWeaponBase> WeaponSlot2;

	int32 CurrentSlot = 0;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> OwnerCharacter;

	friend class APlayerCharacter;

public:
	// Getters and Setters
	FORCEINLINE AWeaponBase* GetWeaponSlot1() const { return WeaponSlot1; }
	FORCEINLINE AWeaponBase* GetWeaponSlot2() const { return WeaponSlot2; }
		
};
