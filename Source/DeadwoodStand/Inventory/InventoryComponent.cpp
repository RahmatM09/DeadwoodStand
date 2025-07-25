// © 2025 Rahmat. Development Build. All rights reserved.


#include "InventoryComponent.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<APlayerCharacter>(GetOwner());
}

bool UInventoryComponent::AddWeapon(AWeaponBase* NewWeapon)
{
	if (!NewWeapon || !OwnerCharacter)
		return false;

	
	NewWeapon->SetActorHiddenInGame(true);
	NewWeapon->SetActorEnableCollision(false);
	NewWeapon->SetActorTickEnabled(false);
	
	if (!WeaponSlot1)
	{
		WeaponSlot1 = NewWeapon;
		OwnerCharacter->UpdateWeaponSlotInfo();
		return true;
	}
	else if (!WeaponSlot2)
	{
		WeaponSlot2 = NewWeapon;
		OwnerCharacter->UpdateWeaponSlotInfo();
		bWeaponsSlotFull = true;
		return true;
	}

	// Inventory full
	return false;
}

void UInventoryComponent::SwapWeapon()
{
	if (!WeaponSlot1 || !WeaponSlot2 || !OwnerCharacter)
		return;

	CurrentSlot = 1 - CurrentSlot;
	TObjectPtr<AWeaponBase> NewWeapon = (CurrentSlot == 0) ? WeaponSlot1 : WeaponSlot2;

	if (NewWeapon)
	{
		OwnerCharacter->CurrentWeapon->Unequip(); // Optional if you have it
		OwnerCharacter->CurrentWeapon = NewWeapon;
		NewWeapon->Equip(OwnerCharacter);
	}
}