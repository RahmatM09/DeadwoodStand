// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_MeleeHit.generated.h"

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API UAnimNotify_MeleeHit : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
