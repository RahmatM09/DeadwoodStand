// © 2025 Rahmat. Development Build. All rights reserved.


#include "AnimNotify_MeleeHit.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DeadwoodStand/Zombie/ReaperZombieCharacter.h"


void UAnimNotify_MeleeHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (!Player) return;

	FVector Start = Player->GetActorLocation();
	FVector Forward = Player->GetActorForwardVector();
	FVector End = Start + Forward * Player->MeleeRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Player);

	FHitResult Hit;
	if (Player->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			AReaperZombieCharacter* Reaper = Cast<AReaperZombieCharacter>(HitActor);
			if (Reaper && Reaper->bIsDodging) // Or Reaper->bIsDodging
			{
				// Reaper dodged the hit
			}
			else
			{
				UGameplayStatics::ApplyPointDamage(HitActor, Player->MeleeDamage, Forward, Hit, Player->GetController(), Player, nullptr);
			}
		}
	}
}
