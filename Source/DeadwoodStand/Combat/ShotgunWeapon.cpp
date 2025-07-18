// © 2025 Rahmat. Development Build. All rights reserved.


#include "ShotgunWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include  "DeadwoodStand/Character/PlayerCharacter.h"

AShotgunWeapon::AShotgunWeapon()
{
    NumPellets = 8;
    PelletSpreadAngle = 5.f;
    Damage = 10.f; // Per pellet
    FireRange = 1200.f;
}

void AShotgunWeapon::Fire()
{
    if (bIsReloading || CurrentAmmo <= 0 || !OwnerCharacter)
		return;

	--CurrentAmmo;
	UpdateHUD();
	OwnerCharacter->StartShootingEffect();
	
	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!PC) return;

	// Get viewpoint from camera
	FVector EyeLocation;
	FRotator EyeRotation;
	PC->GetPlayerViewPoint(EyeLocation, EyeRotation);

	FVector MuzzleLocation = Mesh->GetSocketLocation(MuzzleSocketName);

	for (int32 i = 0; i < NumPellets; ++i)
	{
		// Add random spread
		FRotator SpreadRot = EyeRotation;
		SpreadRot.Yaw += FMath::RandRange(-PelletSpreadAngle, PelletSpreadAngle);
		SpreadRot.Pitch += FMath::RandRange(-PelletSpreadAngle, PelletSpreadAngle);

		// Direction with spread
		FVector ShotDirection = SpreadRot.Vector();

		// Trace from camera to target
		FVector TraceEnd = EyeLocation + (ShotDirection * FireRange);

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(OwnerCharacter);
		Params.bReturnPhysicalMaterial = true;

		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, Params))
		{
			// Apply damage
			UGameplayStatics::ApplyPointDamage(
				Hit.GetActor(),
				Damage,
				ShotDirection,
				Hit,
				OwnerCharacter->GetInstigatorController(),
				this,
				nullptr
			);

			// Impact FX
			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactEffect,
					Hit.ImpactPoint,
					Hit.ImpactNormal.Rotation()
				);
			}

			// Optional: Draw debug line to hit point
			//DrawDebugLine(GetWorld(), MuzzleLocation, Hit.ImpactPoint, FColor::Red, false, 1.0f, 0, 1.0f);
		}
		else
		{
			// Optional: Draw debug line to miss
			//DrawDebugLine(GetWorld(), MuzzleLocation, TraceEnd, FColor::Blue, false, 1.0f, 0, 1.0f);
		}
	}

	// Fire FX (only once)
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, MuzzleSocketName);
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	if (CameraShake && PC)
	{
		PC->ClientStartCameraShake(CameraShake);
	}

	// Auto-reload
	if (CurrentAmmo <= 0)
	{
		Reload();
	}
}