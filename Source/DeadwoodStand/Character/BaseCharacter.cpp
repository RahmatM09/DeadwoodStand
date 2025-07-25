// © 2025 Rahmat. Development Build. All rights reserved.


#include "BaseCharacter.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"



class UBehaviorTreeComponent;

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentHealth = MaxHealth;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ABaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (CurrentHealth <= 0.f || DamageAmount <= 0.f)
	{
		return 0.f;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);

	OnTakeDamage(DamageAmount); // Let Blueprint handle hit effects
	TakeDamage(DamageAmount);    // Let C++ handle hit effects
	
	if (CurrentHealth <= 0.f)
	{
		HandleDeath();       // C++ hook
		OnDeath();           // BP visual/audio logic
	}

	return DamageAmount;
}

void ABaseCharacter::TakeDamage(float DamageAmount)
{
	if (HitReactMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(HitReactMontage);
	}
}

void ABaseCharacter::HandleDeath()
{
	SetLifeSpan(15.f);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	SetActorTickEnabled(false);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));

	if (Controller)
	{
		Controller->StopMovement();

		if (AAIController* AIController = Cast<AAIController>(Controller))
		{
			UBehaviorTreeComponent* BTComp = AIController->FindComponentByClass<UBehaviorTreeComponent>();
			if (BTComp)
			{
				BTComp->StopTree(EBTStopMode::Safe); // Or Force if needed
			}

			// Clear focus and perception
			AIController->ClearFocus(EAIFocusPriority::Default);

			if (UAIPerceptionComponent* Perception = AIController->FindComponentByClass<UAIPerceptionComponent>())
			{
				Perception->ForgetAll();
				Perception->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
				Perception->SetSenseEnabled(UAISense_Hearing::StaticClass(), false);
			}
		}
	}
}

