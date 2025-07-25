// © 2025 Rahmat. Development Build. All rights reserved.


#include "WDPlayerController.h"



void AWDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
}

void AWDPlayerController::Interact()
{
	if (ThisActor)
	{
		IInteractable::Execute_Interact(ThisActor.GetObject(), GetPawn());
	}
}

void AWDPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ActorTrace();
}

void AWDPlayerController::ActorTrace()
{
	// Save last actor
	LastActor = ThisActor;
	ThisActor = nullptr;

	// Trace from camera forward
	FVector Start = PlayerCameraManager->GetCameraLocation();
	FVector End = Start + PlayerCameraManager->GetActorForwardVector() * TraceDistance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPawn());

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	
	if (bHit && HitResult.GetActor() && HitResult.GetActor()->Implements<UInteractable>())
	{
		ThisActor = TScriptInterface<IInteractable>(HitResult.GetActor());
	}

	if (LastActor.GetObject() != ThisActor.GetObject())
	{
		if (LastActor)
		{
			// ✔ Calls OnUnfocus() in BP or C++ cleanly
			IInteractable::Execute_OnUnfocus(LastActor.GetObject());
		}

		if (ThisActor)
		{
			IInteractable::Execute_OnFocus(ThisActor.GetObject());
		}
	}
}