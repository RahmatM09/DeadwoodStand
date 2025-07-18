// © 2025 Rahmat. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeadwoodStand/Interactables/Interactable.h"
#include "Pickup.generated.h"

class USkeletalMeshComponent;
class USphereComponent;

UCLASS()
class DEADWOODSTAND_API APickup : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	APickup();

protected:
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Collision;

public:
	// IInteractable interface overrides
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual void OnFocus_Implementation() override;
	virtual void OnUnfocus_Implementation() override;
	virtual bool CanInteract_Implementation() const override;

private:
	UPROPERTY(EditAnywhere, Category = "Interactable")
	bool bCanInteract = true;
	
};
