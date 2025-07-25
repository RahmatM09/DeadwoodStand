// © 2025 Rahmat. Development Build. All rights reserved.


#include "ProjectileWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"

AProjectileWeapon::AProjectileWeapon()
{
	MuzzleSocketName = TEXT("MuzzleFlash");
	MuzzleVelocity = 2000.f;
}

void AProjectileWeapon::Fire()
{
	if (!OwnerCharacter || bIsReloading || CurrentAmmo <= 0 || !ProjectileClass)
		return;

	// Cache the player controller
	if (!PlayerController)
		PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());

	if (!PlayerController) return;

	--CurrentAmmo;
	UpdateHUD();
	OwnerCharacter->StartShootingEffect();

	// 1. Get camera location and direction
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// 2. Do a line trace from the camera into the world
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 10000.f);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	// 3. Determine the target point
	FVector TargetPoint = Hit.bBlockingHit ? Hit.ImpactPoint : TraceEnd;

	// 4. Calculate direction from muzzle to target
	FVector MuzzleLocation = Mesh->GetSocketLocation(MuzzleSocketName);
	FVector ShootDirection = (TargetPoint - MuzzleLocation).GetSafeNormal();

	// === 🔥 Apply randomized spread ===
	float SpreadRadians = FMath::DegreesToRadians(OwnerCharacter->GetSpreadAmount()); // SpreadAmount is in degrees
	ShootDirection = FMath::VRandCone(ShootDirection, SpreadRadians);

	FRotator MuzzleRotation = ShootDirection.Rotation();

	// 5. Spawn the projectile
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = OwnerCharacter;

	AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

	// 6. Set velocity
	if (UProjectileMovementComponent* ProjMove = Projectile->FindComponentByClass<UProjectileMovementComponent>())
	{
		ProjMove->Velocity = ShootDirection * MuzzleVelocity;
	}

	// 7. Optional fire effects
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, MuzzleSocketName);
	}

	if (CameraShake && PlayerController)
	{
		PlayerController->ClientStartCameraShake(CameraShake);
	}

	// 8. Auto reload if needed
	if (CurrentAmmo <= 0)
	{
		Reload();
	}
}
