// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DeadwoodStand/Interactables/Pickups/Pickup.h"
#include "DeadwoodStand/Combat/WeaponTypes.h"
#include "SpawnPickup.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API ASpawnPickup : public APickup
{
	GENERATED_BODY()

public:
	ASpawnPickup();

protected:
	virtual void Interact_Implementation(AActor* Interactor) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	EWeaponType PickupType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	int32 PickupAmount = 10;
};
