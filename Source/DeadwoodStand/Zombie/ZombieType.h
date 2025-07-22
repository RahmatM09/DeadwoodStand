#pragma once

UENUM(BlueprintType)
enum class EZombieType : uint8
{
	NONE			UMETA(DisplayName = "NONE"),
	Juggernaut		UMETA(DisplayName = "Juggernaut"),
	Screamer		UMETA(DisplayName = "Screamer"),
	Reaper			UMETA(DisplayName = "Reaper"),
};