// © 2025 Rahmat. Development Build. All rights reserved.


#include "PlayerCharacter.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "DeadwoodStand/Player/WDPlayerController.h"
#include "DeadwoodStand/Combat/WeaponBase.h"
#include "DeadwoodStand/UI/BaseHUD.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DeadwoodStand/UI/PlayerStatsWidget.h"
#include  "DeadwoodStand/Inventory/InventoryComponent.h"
#include "DeadwoodStand/SaveLoad/GameSaveData.h"
#include "DeadwoodStand/World/DayNightCycleManage.h"
#include "DeadwoodStand/Zombie/ReaperZombieCharacter.h"
#include "DeadwoodStand/SaveLoad/SaveManager.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and set up Spring Arm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // Distance behind character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));

	
	// Create and attach Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Character movement settings
	bUseControllerRotationYaw = true;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	WalkSpeed = 600.f;
	SprintSpeed = 900.f;
	CurrentWeaponType = EWeaponType::EWT_NoWeapon;
	
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ASaveManager> It(GetWorld()); It; ++It)
	{
		SaveManager = *It;
		break;
	}
	// Bind to Day/Night Cycle Delegate
	for (TActorIterator<ADayNightCycleManage> It(GetWorld()); It; ++It)
	{
		ADayNightCycleManage* CycleManager = *It;
		if (CycleManager)
		{
			CycleManager->OnNightStateChanged.AddDynamic(this, &APlayerCharacter::OnNightStateChanged);
			break;
		}
	}

	// Load game on start
	LoadGameProgress();

	WDPlayerController = Cast<AWDPlayerController>(Controller);
	InitializeInput();
	InitializeHUD();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCrosshairSpread(DeltaTime);
	UpdateStamina(DeltaTime);
	SetAimingCameraFOV(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::StartJump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJump);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprint);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);
		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleCrouch);
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::HandleInteract);
		EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::Fire);
		EnhancedInput->BindAction(FireAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopFire);
		EnhancedInput->BindAction(ReloadAction, ETriggerEvent::Started, this, &APlayerCharacter::Reload);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Started, this, &APlayerCharacter::StartAim);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopAim);
		EnhancedInput->BindAction(WeaponSlot1Action, ETriggerEvent::Started, this, &APlayerCharacter::EquipWeaponSlot1);
		EnhancedInput->BindAction(WeaponSlot2Action, ETriggerEvent::Started, this, &APlayerCharacter::EquipWeaponSlot2);
		EnhancedInput->BindAction(MeleeAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::MeleeAttack);
		EnhancedInput->BindAction(UseHealthAction, ETriggerEvent::Started, this, &APlayerCharacter::UseHealthPickup);
		EnhancedInput->BindAction(UseStaminaAction, ETriggerEvent::Started, this, &APlayerCharacter::UseStaminaPickup);
	}
}

void APlayerCharacter::OnNightStateChanged(bool bIsNight)
{
	if (!bIsNight)
	{
		SaveGameProgress();
	}
}

void APlayerCharacter::SaveGameProgress()
{
	if (SaveManager)
	{
		SaveManager->SaveGame();
	}
}

void APlayerCharacter::LoadGameProgress()
{
	if (SaveManager)
	{
		SaveManager->LoadGame();
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller && MovementVector.SizeSquared() > 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MovementVector.Y);
		AddMovementInput(Right, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(-LookAxis.Y);
}

void APlayerCharacter::StartJump()
{
	Jump();
}

void APlayerCharacter::StopJump(const FInputActionValue& Value)
{
	StopJumping();
}

void APlayerCharacter::StartSprint(const FInputActionValue& Value)
{
	if (CurrentStamina > 0.f)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void APlayerCharacter::StopSprint(const FInputActionValue& Value)
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bCanRecoverStamina = false;

	GetWorldTimerManager().ClearTimer(StaminaRecoveryTimerHandle);
	GetWorldTimerManager().SetTimer(
		StaminaRecoveryTimerHandle,
		this,
		&APlayerCharacter::EnableStaminaRecovery,
		StaminaRecoveryDelay,
		false
	);
}

void APlayerCharacter::TakeDamage(float DamageAmount)
{
	Super::TakeDamage(DamageAmount);
	if (StatsWidget)
	{
		StatsWidget->UpdateHealthBar(CurrentHealth / MaxHealth);
	}
}

void APlayerCharacter::HandleDeath()
{
	Super::HandleDeath();
	if (WDPlayerController)
	{
		WDPlayerController->UnPossess();
	}
}

void APlayerCharacter::EnableStaminaRecovery()
{
	bCanRecoverStamina = true;
}


void APlayerCharacter::ToggleCrouch(const FInputActionValue& Value)
{
	if (bIsCrouched)
		UnCrouch();
	else
		Crouch();
}

void APlayerCharacter::HandleInteract()
{
	if (AWDPlayerController* DWPC = Cast<AWDPlayerController>(GetController()))
	{
		DWPC->Interact();
	}
}

void APlayerCharacter::SetCurrentHealth(float NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	if (StatsWidget)
		StatsWidget->UpdateHealthBar(CurrentHealth / MaxHealth);
}

void APlayerCharacter::SetCurrentWeapon(AWeaponBase* NewWeapon)
{
	if (CurrentWeapon && InventoryComponent)
	{
		InventoryComponent->AddWeapon(NewWeapon);
		return;
	}
	CurrentWeapon = NewWeapon;
	CurrentWeaponType = NewWeapon->WeaponType;
	RefreshHUDForWeapon();
}

void APlayerCharacter::Fire()
{
	if (CurrentWeapon && !CurrentWeapon->IsReloading())
	{
		CurrentWeapon->StartFire();
		
		NoiseEmitter->MakeNoise(this, 1.f, GetActorLocation());
	}
}

void APlayerCharacter::StopFire()
{
	if (CurrentWeapon && !CurrentWeapon->IsReloading())
	{
		CurrentWeapon->StopFire();
	}
}

void APlayerCharacter::Reload()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

void APlayerCharacter::StartAim()
{
	bIsAiming = true;
}

void APlayerCharacter::StopAim()
{
	bIsAiming = false;
}

void APlayerCharacter::EquipWeaponSlot1()
{
	if (InventoryComponent && InventoryComponent->WeaponSlot1)
	{
		EquipWeaponFromSlot(InventoryComponent->WeaponSlot1);
	}
}

void APlayerCharacter::EquipWeaponSlot2()
{
	if (InventoryComponent && InventoryComponent->WeaponSlot2)
	{
		EquipWeaponFromSlot(InventoryComponent->WeaponSlot2);
	}
}

void APlayerCharacter::MeleeAttack()
{
	PerformMeleeAttack();
}

void APlayerCharacter::UseHealthPickup()
{
	if (HealthArray > 0 && CurrentHealth < MaxHealth)
	{
		CurrentHealth += 70;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
		HealthArray--;
		
		StatsWidget->UpdateHealthBar(CurrentHealth / MaxHealth);
		StatsWidget->OnHealthArrayChanged(HealthArray);
	}
}

void APlayerCharacter::UseStaminaPickup()
{
	if (StaminaArray > 0 && CurrentStamina < MaxStamina)
	{
		CurrentStamina += 100;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0, MaxStamina);
		StaminaArray--;
		
		StatsWidget->UpdateStaminaBar(CurrentStamina / MaxStamina);
		StatsWidget->OnStaminaArrayChanged(StaminaArray);
	}
}

void APlayerCharacter::EquipWeaponFromSlot(TObjectPtr<AWeaponBase>& NewWeapon)
{
	if (!NewWeapon || CurrentWeapon == NewWeapon) return;
	
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
		
		TObjectPtr<AWeaponBase> Temp = CurrentWeapon;
		CurrentWeapon = NewWeapon;
		NewWeapon = Temp;
	}
	else
	{
		CurrentWeapon = NewWeapon;
		NewWeapon = nullptr;
	}
	
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(false);
	}
	CurrentWeaponType = CurrentWeapon->WeaponType;
	RefreshHUDForWeapon();
}

void APlayerCharacter::SetAimingCameraFOV(float DeltaTime)
{
	if (!CurrentWeapon) return;
	float TargetFOV = bIsAiming ? AimedFOV : DefaultFOV;
	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, AimInterpSpeed);
	FollowCamera->SetFieldOfView(CurrentFOV);
}

void APlayerCharacter::UpdateWeaponSlotInfo()
{
	if (!StatsWidget || !CurrentWeapon) return;

	if (CurrentWeapon)
	{
		StatsWidget->WeaponSlot1.CurrentAmmo = CurrentWeapon->GetCurrentAmmo();
		StatsWidget->WeaponSlot1.TotalAmmo = CurrentWeapon->GetTotalAmmo();
		StatsWidget->WeaponSlot1.Icon = CurrentWeapon->GetWeaponIcon(); 
		StatsWidget->WeaponSlot1.bNotEmpty = true;
	}
	if (InventoryComponent->WeaponSlot1)
	{
		StatsWidget->WeaponSlot2.CurrentAmmo = InventoryComponent->WeaponSlot1->GetCurrentAmmo();
		StatsWidget->WeaponSlot2.TotalAmmo = InventoryComponent->WeaponSlot1->GetTotalAmmo();
		StatsWidget->WeaponSlot2.Icon = InventoryComponent->WeaponSlot1->GetWeaponIcon();
		StatsWidget->WeaponSlot2.bNotEmpty = true;
	}
	if (InventoryComponent->WeaponSlot2)
	{
		StatsWidget->WeaponSlot3.CurrentAmmo = InventoryComponent->WeaponSlot2->GetCurrentAmmo();
		StatsWidget->WeaponSlot3.TotalAmmo = InventoryComponent->WeaponSlot2->GetTotalAmmo();
		StatsWidget->WeaponSlot3.Icon = InventoryComponent->WeaponSlot2->GetWeaponIcon();
		StatsWidget->WeaponSlot3.bNotEmpty = true;
	}

	StatsWidget->OnWeaponSlotsUpdated();
}

void APlayerCharacter::PerformMeleeAttack()
{
	if (bIsMeleeAttacking) return;

	bIsMeleeAttacking = true;

	if (MeleeAttackMontage && GetMesh())
	{
		PlayAnimMontage(MeleeAttackMontage);
	}

	// Reset attack flag after delay
	GetWorldTimerManager().SetTimer(MeleeResetHandle, this, &APlayerCharacter::ResetMeleeAttackState, 0.5f, false);

}

void APlayerCharacter::ResetMeleeAttackState()
{
	bIsMeleeAttacking = false;
}

TArray<FWeaponData> APlayerCharacter::CollectWeaponData() const
{
	TArray<FWeaponData> CollectedWeapons;

	auto AddWeaponData = [&CollectedWeapons](const AWeaponBase* Weapon)
	{
		if (Weapon)
		{
			FWeaponData Data;
			Data.WeaponType = Weapon->WeaponType;
			Data.CurrentAmmo = Weapon->GetCurrentAmmo();
			Data.ReserveAmmo = Weapon->GetTotalAmmo();
			CollectedWeapons.Add(Data);
		}
	};

	AddWeaponData(CurrentWeapon);
	AddWeaponData(InventoryComponent ? InventoryComponent->WeaponSlot1 : nullptr);
	AddWeaponData(InventoryComponent ? InventoryComponent->WeaponSlot2 : nullptr);

	return CollectedWeapons;
}

void APlayerCharacter::RestoreWeapons(const TArray<FWeaponData>& SavedWeapons)
{
	if (!InventoryComponent) return;

	// --- Step 1: Destroy and Clear Existing Weapons ---
	TArray<AWeaponBase*> WeaponsToDestroy;

	// Collect current weapons
	if (InventoryComponent->WeaponSlot1)
	{
		WeaponsToDestroy.Add(InventoryComponent->WeaponSlot1);
		InventoryComponent->WeaponSlot1 = nullptr;
	}

	if (InventoryComponent->WeaponSlot2)
	{
		WeaponsToDestroy.Add(InventoryComponent->WeaponSlot2);
		InventoryComponent->WeaponSlot2 = nullptr;
	}

	if (CurrentWeapon)
	{
		WeaponsToDestroy.Add(CurrentWeapon);
		CurrentWeapon = nullptr;
	}

	// Destroy old weapon actors
	for (AWeaponBase* Weapon : WeaponsToDestroy)
	{
		if (Weapon)
		{
			Weapon->Destroy();
		}
	}
	
	int32 SlotIndex = 0;

	for (const FWeaponData& WeaponData : SavedWeapons)
	{
		if (!WeaponClassMap.Contains(WeaponData.WeaponType)) continue;

		TSubclassOf<AWeaponBase> WeaponClass = WeaponClassMap[WeaponData.WeaponType];
		if (!WeaponClass) continue;

		AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
		if (!NewWeapon) continue;

		NewWeapon->SetCurrentAmmo(WeaponData.CurrentAmmo);
		NewWeapon->SetTotalAmmo(WeaponData.ReserveAmmo);
		NewWeapon->SetOwner(this);

		// Assign to inventory slots
		if (SlotIndex == 0)
		{
			NewWeapon->Equip(this);
		}
		else if (SlotIndex == 1)
		{
			NewWeapon->Equip(this);
		}
		else if (SlotIndex == 2)
		{
			NewWeapon->Equip(this);
		}

		SlotIndex++;
		if (SlotIndex >= 3) break;
	}
}

void APlayerCharacter::UpdateCrosshairSpread(float DeltaTime)
{
	if (!WDPlayerController) return;
	
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	
	const float MaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float TargetSpread = FMath::GetMappedRangeValueClamped(FVector2D(0.f, MaxSpeed), FVector2D(0.f, 15.f), Speed);
	
	if (!GetCharacterMovement()->IsMovingOnGround())
	{
		TargetSpread += 20.f;
	}
	
	if (bIsShooting)
	{
		TargetSpread += 25.f;
	}
	
	if (bIsAiming)
	{
		TargetSpread -= 10.f; 
	}

	CurrentSpread = 0.f;
	CurrentSpread = FMath::FInterpTo(CurrentSpread, TargetSpread, DeltaTime, 20.f); // 10 is interpolation speed
	
	if (BaseHUD)
	{
		BaseHUD->SetCrosshairSpread(CurrentSpread);
	}
}

void APlayerCharacter::StartShootingEffect()
{
	bIsShooting = true;
	GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &APlayerCharacter::StopShootingEffect, ShootingSpreadDuration, false);
}

void APlayerCharacter::SetHealthArray(int32 NewHealthArray)
{
	HealthArray += NewHealthArray;
	if (StatsWidget)
	{
		StatsWidget->OnHealthArrayChanged(HealthArray);
	}
}

void APlayerCharacter::SetStaminaArray(int32 NewStaminaArray)
{
	StaminaArray += NewStaminaArray;
	if (StatsWidget)
	{
		StatsWidget->OnStaminaArrayChanged(StaminaArray);
	}
}

void APlayerCharacter::StopShootingEffect()
{
	bIsShooting = false;
}

void APlayerCharacter::InitializeInput()
{
	if (!WDPlayerController) return;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(WDPlayerController->GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerCharacter::InitializeHUD()
{
	if (!WDPlayerController) return;

	BaseHUD = Cast<ABaseHUD>(WDPlayerController->GetHUD());
	if (!BaseHUD) return;

	BaseHUD->SetCrosshairWidgetClass(DefautlCrosshairWidgetClass);
	StatsWidget = BaseHUD->GetPlayerStatsWidget();
	if (StatsWidget)
	{
		StatsWidget->InitializeStats(this);
		StatsWidget->OnHealthArrayChanged(HealthArray);
		StatsWidget->OnStaminaArrayChanged(StaminaArray);
	}
	UpdateWeaponSlotInfo();
	if (CurrentWeapon)
	{
		BaseHUD->UpdateWeaponAmmo(CurrentWeapon->GetCurrentAmmo(), CurrentWeapon->GetTotalAmmo());
	}
}

void APlayerCharacter::RefreshHUDForWeapon()
{
	if (!WDPlayerController || !BaseHUD || !StatsWidget || !CurrentWeapon) return;
	
	BaseHUD->SetCrosshairWidgetClass(CurrentWeapon->GetCrosshairWidgetClass());
	UpdateWeaponSlotInfo();
	CurrentWeapon->UpdateHUD();
}

void APlayerCharacter::UpdateStamina(float DeltaTime)
{
	if (bIsSprinting && CurrentStamina > 0.f)
	{
		CurrentStamina -= StaminaDrainRate * DeltaTime;
		if (CurrentStamina <= 0.f)
		{
			CurrentStamina = 0.f;
			StopSprint(0); // Automatically stop sprinting
		}
	}
	else if (bCanRecoverStamina)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRecoveryRate * DeltaTime, 0.f, MaxStamina);
	}
	StatsWidget->UpdateStaminaBar(CurrentStamina / MaxStamina);
}

void APlayerCharacter::MakeMovementNoise()
{
	if (!Controller) return;

	// Get the velocity magnitude
	const float Speed = GetVelocity().Size();

	// Define thresholds
	const float WalkSpeedThreshold = 300.f;
	const float SprintSpeedThreshold = 800.f;

	// Don't make noise if crouching or walking
	if (bIsCrouched || Speed < WalkSpeedThreshold)
	{
		return;
	}

	// Small noise for running
	if (Speed >= WalkSpeedThreshold && Speed < SprintSpeedThreshold)
	{
		MakeNoise(.5, this, GetActorLocation());

	}
	else if (Speed >= SprintSpeedThreshold)
	{
		MakeNoise(0.8, this, GetActorLocation());
	}
}