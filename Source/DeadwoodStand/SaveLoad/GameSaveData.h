// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DeadwoodStand/Combat/WeaponTypes.h"
#include "GameFramework/SaveGame.h"
#include "DeadwoodStand/Zombie/ZombieType.h"
#include "GameSaveData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY()
	EWeaponType WeaponType;

	UPROPERTY()
	int32 CurrentAmmo;

	UPROPERTY()
	int32 ReserveAmmo;
};

USTRUCT(BlueprintType)
struct FZombieData
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	EZombieType ZombieType;

	UPROPERTY()
	float CurrentHealth;
};



/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API UGameSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 DaysSurvived;

	UPROPERTY()
	float PlayerHealth;

	UPROPERTY()
	float PlayerStamina;

	UPROPERTY()
	int32 HealthArray;

	UPROPERTY()
	int32 StaminaArray;

	UPROPERTY()
	TArray<FWeaponData> Weapons;

	UPROPERTY()
	TArray<FZombieData> Zombies;
};
