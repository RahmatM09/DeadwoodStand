// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class DEADWOODSTAND_API ASpawner : public AActor
{
	GENERATED_BODY()
public:
	ASpawner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** True if it's currently night */
	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
	bool bNight = false;

	/** Reset when new night starts */
	UFUNCTION()
	void HandleNightTransition(bool bIsNowNight);
	
	/** Actor class to spawn */
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> ActorToSpawn;

	/** Number of times to spawn */
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (ClampMin = 1))
	int32 SpawnCount = 5;

	/** Time between spawns (in seconds) */
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (ClampMin = 0.1f))
	float SpawnInterval = 20.f;

	/** Should spawn only during night */
	UPROPERTY(EditAnywhere, Category = "Spawner")
	bool bSpawnOnlyAtNight = true;

	/** Call to reset and restart the spawn cycle */
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void ResetSpawner();

private:
	int32 SpawnedCount = 0;
	FTimerHandle SpawnTimerHandle;

	void StartSpawning();
	void SpawnActor();

};
