// © 2025 Rahmat. Development Build. All rights reserved.


#include "SpawnPickup.h"
#include "DeadwoodStand/Inventory/InventoryComponent.h"
#include "DeadwoodStand/Combat/WeaponBase.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"

ASpawnPickup::ASpawnPickup()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawnPickup::Interact_Implementation(AActor* Interactor)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Interactor);
	if (!Player) return;

	switch (PickupType)
	{
	case EWeaponType::EWT_Health:
		Player->SetHealthArray(1);
		Destroy();
		break;

	case EWeaponType::EWT_Food:
		Player->SetStaminaArray(1);
		Destroy();
		break;

	case EWeaponType::EWT_Rifle:
	case EWeaponType::EWT_Shotgun:
	case EWeaponType::EWT_Pistol:
	case EWeaponType::EWT_Grenade:
		{
			TArray<AWeaponBase*> Weapons = { Player->GetCurrentWeapon(), Player->GetInventoryComponent()->GetWeaponSlot1(), Player->GetInventoryComponent()->GetWeaponSlot2() };
			for (AWeaponBase* Weapon : Weapons)
			{
				if (Weapon && Weapon->WeaponType == (PickupType))
				{
					Weapon->AddAmmo(PickupAmount);
					if (Weapon == Player->GetCurrentWeapon())
						Weapon->UpdateHUD();
					Destroy();
					break;
				}
			}
			break;
		}

	default:
		break;
	}
	Player->UpdateWeaponSlotInfo();
}

