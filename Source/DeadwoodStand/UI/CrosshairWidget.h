// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/** Spread value to adjust crosshair layout */
	UPROPERTY(BlueprintReadWrite, Category = "Crosshair")
	float Spread;

	/** Override in Blueprint to update color dynamically */
	UFUNCTION(BlueprintImplementableEvent, Category = "Crosshair")
	void UpdateColor(FLinearColor NewColor);
};
