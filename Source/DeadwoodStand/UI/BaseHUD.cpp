// © 2025 Rahmat. Development Build. All rights reserved.


#include "BaseHUD.h"
#include "CrosshairWidget.h"
#include "DeadwoodStand/UI/PlayerStatsWidget.h"

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();
	if (PlayerStatWidgetClass)
	{
		PlayerStatWidget = CreateWidget<UPlayerStatsWidget>(GetWorld(), PlayerStatWidgetClass);
		if (PlayerStatWidget)
		{
			PlayerStatWidget->AddToViewport();
		}
	}
}

void ABaseHUD::SetCrosshairWidgetClass(TSubclassOf<UCrosshairWidget> NewWidgetClass)
{
	if (CurrentCrosshair)
	{
		CurrentCrosshair->RemoveFromParent();
		CurrentCrosshair = nullptr;
	}

	if (NewWidgetClass)
	{
		CurrentCrosshair = CreateWidget<UCrosshairWidget>(GetWorld(), NewWidgetClass);
		if (CurrentCrosshair)
		{
			CurrentCrosshair->AddToViewport();
			CurrentCrosshairClass = NewWidgetClass;
		}
	}
}

void ABaseHUD::SetCrosshairSpread(float SpreadValue)
{
	if (CurrentCrosshair)
	{
		CurrentCrosshair->Spread = SpreadValue;
	}
}

void ABaseHUD::SetCrosshairColor(FLinearColor NewColor)
{
	if (CurrentCrosshair)
	{
		CurrentCrosshair->UpdateColor(NewColor);
	}
}

void ABaseHUD::UpdateWeaponAmmo(int32 CurrentAmmo, int32 TotalAmmo)
{

	if (PlayerStatWidget)
	{
		PlayerStatWidget->UpdateAmmoInfo(CurrentAmmo, TotalAmmo);
	}
}


