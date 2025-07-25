// © 2025 Rahmat. Development Build. All rights reserved.


#include "PlayerStatsWidget.h"
#include "Components/ProgressBar.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"

void UPlayerStatsWidget::InitializeStats(APlayerCharacter* InCharacter)
{
	PlayerRef = InCharacter;

	UpdateHealthBar(PlayerRef->GetCurrentHealth() / PlayerRef->GetMaxHealth());
	UpdateStaminaBar(PlayerRef->GetCurrentStamina() / PlayerRef->GetMaxStamina());
}

void UPlayerStatsWidget::UpdateHealthBar(float Percent)
{
	HealthValue = Percent;
}

void UPlayerStatsWidget::UpdateStaminaBar(float Percent)
{
	StaminaValue = Percent;
}

void UPlayerStatsWidget::UpdateAmmoInfo_Implementation(int32 CurrentAmmo, int32 TotalAmmo)
{
	
}
