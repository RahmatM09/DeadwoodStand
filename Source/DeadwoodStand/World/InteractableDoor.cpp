// © 2025 Rahmat. Development Build. All rights reserved.


#include "InteractableDoor.h"
#include "TimerManager.h"

AInteractableDoor::AInteractableDoor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();

	ClosedRotation = GetActorRotation();
	OpenRotation = ClosedRotation + FRotator(0.f, OpenAngle, 0.f);
	TargetRotation = ClosedRotation;
}

void AInteractableDoor::Interact_Implementation(AActor* Interactor)
{
	if (!bIsOpen)
	{
		OpenDoor();
	}
}

void AInteractableDoor::OpenDoor()
{
	bIsOpen = true;
	TargetRotation = OpenRotation;

	// Start auto-close timer
	GetWorldTimerManager().SetTimer(AutoCloseTimer, this, &AInteractableDoor::CloseDoor, AutoCloseDelay, false);
}

void AInteractableDoor::CloseDoor()
{
	bIsOpen = false;
	TargetRotation = ClosedRotation;
}

void AInteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Smooth rotation to target
	FRotator CurrentRotation = GetActorRotation();
	if (!CurrentRotation.Equals(TargetRotation, 0.5f))
	{
		FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, OpenSpeed);
		SetActorRotation(NewRotation);
	}
}
