// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightCycleManage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNightStateChanged, bool, bIsNight);


class ADirectionalLight;
class UCurveLinearColor;


UCLASS()
class DEADWOODSTAND_API ADayNightCycleManage : public AActor
{
	GENERATED_BODY()
public:	
	ADayNightCycleManage();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Event fired when day turns into night or vice versa */
	UPROPERTY(BlueprintAssignable, Category = "DayNight")
	FOnNightStateChanged OnNightStateChanged;

	// Directional Light (sun)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	TObjectPtr<ADirectionalLight> SunLight;

	// Rotation speed (degrees per second)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	float SunRotationSpeed = 1.0f;

	// Time of day, 0.0 = midnight, 0.5 = noon, 1.0 = full day
	UPROPERTY(BlueprintReadOnly, Category = "DayNight")
	float TimeOfDay = 0.f;
	bool bWasNight = false;

	// Full day duration in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	float FullDayDuration = 600.f;

	// Light intensity range
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	float MaxIntensity = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	float MinIntensity = 0.0f;

	// Light color curve (optional)
	UPROPERTY(EditAnywhere, Category = "DayNight")
	TObjectPtr<UCurveLinearColor> LightColorCurve;

private:
	void UpdateSunRotation(float DeltaTime);
	void UpdateSunLight();
	

};
