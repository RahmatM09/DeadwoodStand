// © 2025 Rahmat. Development Build. All rights reserved.


#include "ReaperZombieCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"

AReaperZombieCharacter::AReaperZombieCharacter()
{
	// Fast and responsive
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	MaxHealth = 200.f;
	CurrentHealth = MaxHealth;
}

void AReaperZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AReaperZombieCharacter::PerformReaperAttack()
{
	if (bIsAttacking || !AttackMontage) return;

	bIsAttacking = true;
	PlayAnimMontage(AttackMontage);

	// Delay point damage to sync with montage
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		// Line trace forward
		FVector Start = GetActorLocation();
		FVector End = Start + GetActorForwardVector() * MeleeAttackRange;

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params))
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(Hit.GetActor());
			if (Player)
			{
				UGameplayStatics::ApplyPointDamage(Player, MeleeAttackDamage, GetActorForwardVector(), Hit, GetController(), this, nullptr);
			}
		}

		bIsAttacking = false;

	}, 0.6f, false); // adjust delay to match montage impact
}

void AReaperZombieCharacter::TryDodge()
{
	if (!DodgeMontage) return;
	if (bIsDodging) return;

	bIsDodging = true;
	
	float RandomChance = FMath::FRand();
	if (RandomChance < 0.3f)
	{
		PlayAnimMontage(DodgeMontage);
	}
	GetWorld()->GetTimerManager().SetTimer(DodgeCooldownHandle, this,  &AReaperZombieCharacter::StopDodging, 0.6f, false);
}

void AReaperZombieCharacter::StopDodging()
{
	bIsDodging = false;
}