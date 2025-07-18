// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEADWOODSTAND_API IInteractable
{
	GENERATED_BODY()

public:
	/** Called when player interacts */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void Interact(AActor* Interactor);

	/** Called when player looks at the object */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void OnFocus();

	/** Called when player looks away */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void OnUnfocus();

	/** Should return whether interaction is allowed */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	bool CanInteract() const;
	
};
