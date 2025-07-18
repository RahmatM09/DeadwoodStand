// © 2025 Rahmat. Development Build. All rights reserved.


#include "ZombieBaseCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
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