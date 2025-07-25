// © 2025 Rahmat. Development Build. All rights reserved.


#include "Spawner.h"

#include "DayNightCycleManage.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "DeadwoodStand/Zombie/ZombieBaseCharacter.h"


ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	// Try to find the DayNightCycleManager in world
	TArray<AActor*> FoundManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADayNightCycleManage::StaticClass(), FoundManagers);

	if (FoundManagers.Num() > 0)
	{
		ADayNightCycleManage* Manager = Cast<ADayNightCycleManage>(FoundManagers[0]);
		if (Manager)
		{
			Manager->OnNightStateChanged.AddDynamic(this, &ASpawner::HandleNightTransition);
		}
	}
	
	StartSpawning();
}

void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawner::HandleNightTransition(bool bIsNowNight)
{
	bNight = bIsNowNight;

	if (bNight)
	{
		ResetSpawner(); // Restart spawn cycle at night
	}
}

void ASpawner::StartSpawning()
{
	SpawnedCount = 0;

	if (!ActorToSpawn) return;

	// If only spawn at night, and it's currently day — wait
	if (bSpawnOnlyAtNight && !bNight)
	{
		// Try again after some time
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawner::StartSpawning, 5.f, false);
		return;
	}

	// Begin spawn cycle
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawner::SpawnActor, SpawnInterval, true);
}

void ASpawner::SpawnActor()
{
	if (SpawnedCount >= SpawnCount)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	if (!ActorToSpawn) return;

	// Check if any actors of the same class are within 70 units
	TArray<AActor*> OverlappingActors;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // You can also use ECC_WorldDynamic if needed
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this); // Ignore the spawner itself

	bool bHasOverlap = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		70.f,                      // Radius
		ObjectTypes,               // Object types to check
		ActorToSpawn,              // Only overlap actors of this class
		ActorsToIgnore,            // Ignored actors
		OverlappingActors          // Output array
	);

	if (bHasOverlap && OverlappingActors.Num() > 0)
	{
		// Another actor of the same class is nearby, so don't spawn
		return;
	}

	// Optional: Re-check day/night for each spawn
	if (bSpawnOnlyAtNight && !bNight) return;

	FActorSpawnParameters SpawnParams;
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

	if (Spawned)
	{
		SpawnedCount++;

		if (AZombieBaseCharacter* Zombie = Cast<AZombieBaseCharacter>(Spawned))
		{
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

void ASpawner::ResetSpawner()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	SpawnedCount = 0;
	StartSpawning();
}