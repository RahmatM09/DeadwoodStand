// © 2025 Rahmat. Development Build. All rights reserved.


#include "DayNightCycleManage.h"

#include "EngineUtils.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Curves/CurveLinearColor.h"


ADayNightCycleManage::ADayNightCycleManage()
{
	PrimaryActorTick.bCanEverTick = true;

	;
}

void ADayNightCycleManage::BeginPlay()
{
	Super::BeginPlay();

	if (!SunLight)
	{
		for (TActorIterator<ADirectionalLight> It(GetWorld()); It; ++It)
		{
			SunLight = *It;
			break;
		}
	}
}

void ADayNightCycleManage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSunRotation(DeltaTime);
	UpdateSunLight();
}

void ADayNightCycleManage::UpdateSunRotation(float DeltaTime)
{
	if (!SunLight) return;

	TimeOfDay += DeltaTime / FullDayDuration;
	if (TimeOfDay > 1.f) TimeOfDay -= 1.f;

	float SunPitch = TimeOfDay * 360.f - 90.f; // -90 to 270 degrees (sunrise to sunset)
	FRotator NewRotation = FRotator(SunPitch, 90.f, 0.f);
	SunLight->SetActorRotation(NewRotation);

	// Simple threshold: night is when sun is below horizon
	bool bIsNight = !(TimeOfDay < 0.25f || TimeOfDay > 0.75f);

	if (bWasNight != bIsNight)
	{
		bWasNight = bIsNight;
		OnNightStateChanged.Broadcast(bIsNight);
	}
}

void ADayNightCycleManage::UpdateSunLight()
{
	if (!SunLight || !SunLight->GetLightComponent()) return;

	float IntensityFactor = FMath::Clamp(FMath::Cos(TimeOfDay * 2 * PI), 0.0f, 1.0f); // Max at noon

	float NewIntensity = FMath::Lerp(MinIntensity, MaxIntensity, IntensityFactor);
	SunLight->GetLightComponent()->SetIntensity(NewIntensity);

	if (LightColorCurve)
	{
		FLinearColor NewColor = LightColorCurve->GetLinearColorValue(TimeOfDay);
		SunLight->GetLightComponent()->SetLightColor(NewColor);
	}
}