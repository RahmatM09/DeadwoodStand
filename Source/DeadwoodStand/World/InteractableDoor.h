// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DeadwoodStand/Interactables/Pickups/Pickup.h"
#include "InteractableDoor.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API AInteractableDoor : public APickup
{
	GENERATED_BODY()

public:
	AInteractableDoor();

protected:
	virtual void BeginPlay() override;

	// Whether the door is open
	bool bIsOpen = false;

	// Original and target rotation
	FRotator ClosedRotation;
	FRotator OpenRotation;

	// Smooth open/close speed
	UPROPERTY(EditAnywhere, Category = "Door")
	float OpenSpeed = 90.f;

	// How far to rotate when opening
	UPROPERTY(EditAnywhere, Category = "Door")
	float OpenAngle = 90.f;

	// Time before closing again
	UPROPERTY(EditAnywhere, Category = "Door")
	float AutoCloseDelay = 15.f;

	FTimerHandle AutoCloseTimer;

public:
	// Interaction
	virtual void Interact_Implementation(AActor* Interactor) override;

	virtual void Tick(float DeltaTime) override;

private:
	void OpenDoor();
	void CloseDoor();

	UPROPERTY()
	FRotator TargetRotation;
};
