// © 2025 Rahmat. Development Build. All rights reserved.


#include "ZombieBaseCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "ReaperZombieCharacter.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"

AZombieBaseCharacter::AZombieBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set default movement
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void AZombieBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AZombieBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

float AZombieBaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	FHitResult Hit;
	FPointDamageEvent* PointDamageEvent = nullptr;

	// Check if it's a point damage type to access HitInfo
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
		Hit = PointDamageEvent->HitInfo;
	}

	// Check if hit bone is "head"
	if (Hit.BoneName == FName("head") || Hit.BoneName == FName("Head"))
	{
		DamageAmount = 100.f; // Force 100 damage for headshots
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
}

void AZombieBaseCharacter::PlayScreechFX()
{
	UGameplayStatics::SpawnSoundAtLocation(this, ScreechSound, GetActorLocation());
	
	MakeNoise(1, this, GetActorLocation());	
}
		