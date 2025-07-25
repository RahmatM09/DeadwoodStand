// © 2025 Rahmat. Development Build. All rights reserved.


#include "SaveManager.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"
#include "DeadwoodStand/Zombie/ReaperZombieCharacter.h"
#include "DeadwoodStand/Zombie/ZombieBaseCharacter.h"

void ASaveManager::BeginPlay()
{
	Super::BeginPlay();
	//LoadGame();
}

void ASaveManager::SaveGame()
{
	UGameSaveData* SaveData = Cast<UGameSaveData>(UGameplayStatics::CreateSaveGameObject(UGameSaveData::StaticClass()));

	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player) return;

	// Player data
	SaveData->PlayerHealth = Player->GetCurrentHealth();
	SaveData->PlayerStamina = Player->GetCurrentStamina();
	SaveData->HealthArray = Player->GetHealthArray();
	SaveData->StaminaArray = Player->GetStaminaArray();
	SaveData->DaysSurvived = Player->GetDaysSurvived();

	// Weapons
	TArray<FWeaponData> CollectedWeapons = Player->CollectWeaponData();
	SaveData->Weapons = CollectedWeapons;

	// Zombies
	TArray<AActor*> ZombieActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombieBaseCharacter::StaticClass(), ZombieActors);
	for (AActor* Actor : ZombieActors)
	{
		AZombieBaseCharacter* Zombie = Cast<AZombieBaseCharacter>(Actor);
		if (Zombie)
		{
			FZombieData ZData;
			ZData.Location = Zombie->GetActorLocation();
			ZData.Rotation = Zombie->GetActorRotation();
			ZData.ZombieType = Zombie->GetZombieType();
			ZData.CurrentHealth = Zombie->GetCurrentHealth();
			SaveData->Zombies.Add(ZData);
		}
	}
	
	UGameplayStatics::SaveGameToSlot(SaveData, SaveSlotName, SaveUserIndex);
}

void ASaveManager::LoadGame()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex)) return;

	UGameSaveData* SaveData = Cast<UGameSaveData>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));
	if (!SaveData) return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player) return;

	TArray<AActor*> ZombieActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombieBaseCharacter::StaticClass(), ZombieActors);
	for (AActor* Actor : ZombieActors)
		Actor->Destroy();

	// Restore Player
	Player->SetCurrentHealth(SaveData->PlayerHealth);
	Player->SetCurrentStamina(SaveData->PlayerStamina);
	Player->ResetHealthArray(SaveData->HealthArray);
	Player->ResetStaminaArray(SaveData->StaminaArray);
	Player->SetDaysSurvived(SaveData->DaysSurvived);
	Player->RestoreWeapons(SaveData->Weapons);

	// Restore Zombies
	for (const FZombieData& Z : SaveData->Zombies)
	{
		TSubclassOf<AActor> ZombieClass;

		switch (Z.ZombieType)
		{
			case EZombieType::Reaper: ZombieClass = ReaperZombieClass; break;
			case EZombieType::Screamer: ZombieClass = ScreamerZombieClass; break;
			case EZombieType::Juggernaut: ZombieClass = JaggernautZombieClass; break;
			default: ZombieClass = nullptr; break;
		}
		if (!ZombieClass) continue;

		AActor* Spawned = GetWorld()->SpawnActor(ZombieClass, &Z.Location, &Z.Rotation);
		AZombieBaseCharacter* Zombie = Cast<AZombieBaseCharacter>(Spawned);
		if (Zombie)
		{
			Zombie->SetCurrentHealth(Z.CurrentHealth);
			Zombie->GetMesh()->SetSimulatePhysics(true);
			Zombie->GetMesh()->WakeAllRigidBodies();
			Zombie->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			Zombie->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
			if (!Zombie->GetController())
			{
				Zombie->SpawnDefaultController();
			}
		}
	}
}

void ASaveManager::AutoSaveAtDayStart()
{
	SaveGame();
}
