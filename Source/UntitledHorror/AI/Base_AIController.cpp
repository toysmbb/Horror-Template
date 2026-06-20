// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_AIController.h"
#include "BehaviorTree/BehaviorTree.h"

ABase_AIController::ABase_AIController()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPercepComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPercerpComp"));
	SenseSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseSight"));
	SenseHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SenseHearing"));
	if (AIPercepComp)
	{
		if (SenseSight)
			AIPercepComp->ConfigureSense(*SenseSight);
		if (SenseHearing)
			AIPercepComp->ConfigureSense(*SenseHearing);

		AIPercepComp->SetDominantSense(UAISense_Sight::StaticClass());
	}
}

void ABase_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunBehaviorTree(BT);
}
