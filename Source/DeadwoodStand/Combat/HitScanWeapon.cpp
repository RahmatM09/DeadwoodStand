// © 2025 Rahmat. Development Build. All rights reserved.


#include "HitScanWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/PlayerController.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"

AHitScanWeapon::AHitScanWeapon()
{
    WeaponType = EWeaponType::EWT_Pistol;
}

void AHitScanWeapon::Fire()
{
    if (!OwnerCharacter || CurrentAmmo <= 0)
        return;

    --CurrentAmmo;
    UpdateHUD();
    OwnerCharacter->StartShootingEffect();

    // 1. Get camera location and direction
    if (!PlayerController)
        PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
    if (!PlayerController) return;

    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector ShotDirection = CameraRotation.Vector();

    // === 🔥 Apply inaccuracy based on spread ===
    float SpreadRadians = FMath::DegreesToRadians(OwnerCharacter->GetSpreadAmount()); // SpreadAmount is in degrees
    ShotDirection = FMath::VRandCone(ShotDirection, SpreadRadians);

    // 2. Line Trace
    FVector TraceStart = CameraLocation;
    FVector TraceEnd = TraceStart + (ShotDirection * 10000.f);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerCharacter);
    Params.AddIgnoredActor(this);

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);

    // 3. Handle impact
    HandleImpact(HitResult);

    // 4. Muzzle Flash
    if (MuzzleFlash)
    {
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlash"));
    }

    // 5. Fire Sound
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    // 6. Camera Shake
    if (PlayerController && CameraShake)
    {
        PlayerController->ClientStartCameraShake(CameraShake);
    }
}

void AHitScanWeapon::PerformLineTrace(FHitResult& HitOut)
{
    if (!PlayerController) return;
    
    FVector Start = PlayerController->PlayerCameraManager->GetCameraLocation();
    FVector End = Start + PlayerController->PlayerCameraManager->GetActorForwardVector() * FireRange;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(OwnerCharacter);
    Params.bReturnPhysicalMaterial = true;

    GetWorld()->LineTraceSingleByChannel(HitOut, Start, End, ECC_Visibility, Params);
}

void AHitScanWeapon::HandleImpact(const FHitResult& Hit)
{
    if (Hit.bBlockingHit)
    {
        // Deal damage
        UGameplayStatics::ApplyPointDamage(
            Hit.GetActor(),
            Damage,
            Hit.TraceStart - Hit.TraceEnd,
            Hit,
            OwnerCharacter->GetInstigatorController(),
            this,
            nullptr
        );

        // Spawn impact effect
        if (ImpactEffect)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
        }
    }
}
