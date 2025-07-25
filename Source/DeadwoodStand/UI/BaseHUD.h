// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UCrosshairWidget;
class UPlayerStatsWidget;
/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	void SetCrosshairWidgetClass(TSubclassOf<UCrosshairWidget> NewWidgetClass);
	void SetCrosshairSpread(float SpreadValue);
	void SetCrosshairColor(FLinearColor NewColor);
	UFUNCTION(BlueprintCallable)
	void UpdateWeaponAmmo(int32 CurrentAmmo, int32 TotalAmmo);
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TObjectPtr<UCrosshairWidget> CurrentCrosshair;

	UPROPERTY()
	TSubclassOf<UCrosshairWidget> CurrentCrosshairClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerStatsWidget> PlayerStatWidgetClass;

	UPROPERTY()
	TObjectPtr<UPlayerStatsWidget> PlayerStatWidget;

public:
	// Setters and Getters
	FORCEINLINE UPlayerStatsWidget* GetPlayerStatsWidget() const { return PlayerStatWidget; }
};
