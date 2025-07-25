// © 2025 Rahmat. Development Build. All rights reserved.


#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"
#include "DeadwoodStand/Inventory/InventoryComponent.h"
#include "DeadwoodStand/UI/BaseHUD.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponType = EWeaponType::EWT_Pistol;
	
	MaxAmmoInClip = 30;
	TotalAmmo = 50;
	AttachSocketName = TEXT("Weapon_hand_r_Socket");
	bIsEquipped = false;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmoInClip;

	if (OwnerCharacter)
	{
		PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	}
	UpdateHUD();
}

void AWeaponBase::Interact_Implementation(AActor* Interactor)
{
	Super::Interact_Implementation(Interactor);
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Interactor);
	Equip(PlayerCharacter);
}

void AWeaponBase::Equip(APlayerCharacter* NewOwner)
{
	if (!NewOwner) return;
	
	OwnerCharacter = NewOwner;

	if (OwnerCharacter && OwnerCharacter->GetInventoryComponent())
	{
		if (OwnerCharacter->GetInventoryComponent()->bWeaponsSlotFull)
			return;
	}
	
	bIsEquipped = true;
	OwnerCharacter->SetCurrentWeapon(this);

	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketName);
	SetActorEnableCollision(false);
}

void AWeaponBase::Unequip()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	bIsEquipped = false;
	OwnerCharacter = nullptr;
	SetActorEnableCollision(true);
}

void AWeaponBase::Reload()
{
	if (bIsReloading || CurrentAmmo == MaxAmmoInClip || TotalAmmo <= 0)
		return;

	bIsReloading = true;
	UE_LOG(LogTemp, Log, TEXT("Reloading..."));

	float AnimDuration = ReloadTime;
	if (ReloadMontage && OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimDuration = AnimInstance->Montage_Play(ReloadMontage);
		}
	}
	
	ReloadTime = AnimDuration;
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponBase::FinishReload, ReloadTime, false);
}

bool AWeaponBase::CanReload() const
{
	return CurrentAmmo < MaxAmmoInClip;
}

void AWeaponBase::Fire()
{
	if (bIsReloading || CurrentAmmo <= 0)
		return;

	if (FireMontage && OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(FireMontage);
		}
	}
	UpdateHUD();
	if (CurrentAmmo == 0)
		Reload();
}

void AWeaponBase::StartFire()
{
	if (bIsAutomatic)
	{
		// Start repeating fire
		Fire(); // Immediate first shot
		GetWorldTimerManager().SetTimer(AutoFireHandle, this, &AWeaponBase::Fire, FireRate, true);
	}
	else
	{
		Fire(); // Single shot
	}
}

void AWeaponBase::StopFire()
{
	if (bIsAutomatic)
	{
		GetWorldTimerManager().ClearTimer(AutoFireHandle);
	}
}

void AWeaponBase::UpdateHUD()
{
	if (!OwnerCharacter) return;

	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (!PC) return;

	ABaseHUD* HUD = Cast<ABaseHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->UpdateWeaponAmmo(CurrentAmmo, TotalAmmo);
	}
}

void AWeaponBase::AddAmmo(int32 Amount)
{
	if (Amount <= 0) return;
	TotalAmmo += Amount;
}

void AWeaponBase::FinishReload()
{
	StopFire();
	int32 AmmoNeeded = MaxAmmoInClip - CurrentAmmo;
	int32 AmmoToReload = FMath::Min(AmmoNeeded, TotalAmmo);

	CurrentAmmo += AmmoToReload;
	TotalAmmo -= AmmoToReload;
	bIsReloading = false;
	UpdateHUD();
	OwnerCharacter->UpdateWeaponSlotInfo();
	UE_LOG(LogTemp, Log, TEXT("Reloaded. CurrentAmmo: %d, TotalAmmo: %d"), CurrentAmmo, TotalAmmo);

}
