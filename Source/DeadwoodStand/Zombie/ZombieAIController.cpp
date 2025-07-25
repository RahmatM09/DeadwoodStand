// © 2025 Rahmat. Development Build. All rights reserved.


#include "ZombieAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeadwoodStand/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DeadwoodStand/Zombie/ZombieBaseCharacter.h"

AZombieAIController::AZombieAIController()
{
	// Init Perception
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1200.f;
	HearingConfig->SetMaxAge(3.f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->ConfigureSense(*HearingConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AZombieAIController::OnTargetPerceptionUpdated);

	// Init Blackboard
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AZombieAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AZombieAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		UBlackboardComponent* BlackboardRaw = Blackboard.Get();
		if (UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardRaw))
		{
			Blackboard = BlackboardRaw;
			RunBehaviorTree(BehaviorTreeAsset);
		}
	}
}

APawn* AZombieAIController::GetTargetPawn() const
{
	if (!Blackboard) return nullptr;
	UObject* TargetObj = Blackboard->GetValueAsObject("TargetActor");
	return Cast<APawn>(TargetObj);
}

void AZombieAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor->IsA<APlayerCharacter>()) return;
	if (Blackboard)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			Blackboard->SetValueAsObject(FName("TargetActor"), Actor);
			Blackboard->SetValueAsBool(bHasLineOfSightKey, true);
		}
		else
		{
			Blackboard->ClearValue(FName("TargetActor"));
			Blackboard->SetValueAsBool(bHasLineOfSightKey, false);
		}
	}
}
