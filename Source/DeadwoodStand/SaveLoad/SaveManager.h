// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSaveData.h"
#include "SaveManager.generated.h"

class AZombieBaseCharacter;

UCLASS()
class DEADWOODSTAND_API ASaveManager : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame();
	void AutoSaveAtDayStart();

protected:
	virtual void BeginPlay() override;

private:
	FString SaveSlotName = TEXT("DeadwoodSave");
	int32 SaveUserIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Save|Zombie")
	TSubclassOf<AZombieBaseCharacter> ScreamerZombieClass;

	UPROPERTY(EditAnywhere, Category = "Save|Zombie")
	TSubclassOf<AZombieBaseCharacter> ReaperZombieClass;

	UPROPERTY(EditAnywhere, Category = "Save|Zombie")
	TSubclassOf<AZombieBaseCharacter> JaggernautZombieClass;

};
