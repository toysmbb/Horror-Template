// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_DM.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseNPC.h"
#include "Kismet/GameplayStatics.h"

void AAIC_DM::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!SenseSight || !SenseHearing)
	{
		UE_LOG(LogTemp, Warning, TEXT("SenseSight or SenseHearing is nullptr"));
		return;
	}

	UBlackboardComponent* BB = Cast<UBlackboardComponent>(GetBlackboardComponent());
	if(!BB)
	{
		UE_LOG(LogTemp, Warning, TEXT("BB is nullptr"));
		return;
	}
	FAISenseID SightID = SenseSight->GetSenseID();
	FAISenseID HearingID = SenseHearing->GetSenseID();

	if (Stimulus.WasSuccessfullySensed())
	{
	  if(Stimulus.Type == SightID)
	  {
		  UE_LOG(LogTemp, Warning, TEXT("NPC seen player"));
		  CurrentState = EDMState::Chase;
		  ABaseNPC* NPC = Cast<ABaseNPC>(GetPawn());
		  NPC->GetCharacterMovement()->MaxWalkSpeed = 350.0f;
		  BB->SetValueAsEnum(FName("StateKey"), static_cast<uint8>(EDMState::Chase));
	  }
	  if (Stimulus.Type == HearingID && CurrentState != EDMState::Chase)
	  {
		  BB->SetValueAsVector(FName("HearingLocation"), Stimulus.StimulusLocation);
		  CurrentState = EDMState::Suspect;
		  BB->SetValueAsEnum(FName("StateKey"), static_cast<uint8>(EDMState::Suspect));
	  }
	}
	else
	{
		if (Stimulus.Type == SenseSight->GetSenseID()) 
		{
			LastLocation = Stimulus.StimulusLocation;
			CurrentState = EDMState::Search;
			BB->SetValueAsEnum(FName("StateKey"), static_cast<uint8>(EDMState::Search));
		}
	}
}

AAIC_DM::AAIC_DM()
{
	if (AIPercepComp)
	{
		if (SenseSight)
			AIPercepComp->ConfigureSense(*SenseSight);
		if (SenseHearing)
			AIPercepComp->ConfigureSense(*SenseHearing);

		AIPercepComp->SetDominantSense(UAISense_Sight::StaticClass());
	}
}

void AAIC_DM::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AIPercepComp->OnTargetPerceptionUpdated.AddDynamic(this, &AAIC_DM::OnPerceptionUpdated);
}

void AAIC_DM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EDMState State = static_cast<EDMState>(
		Blackboard->GetValueAsEnum("StateKey")
		);

	FString StateString;

	switch (State)
	{
	case EDMState::Patrol:
		StateString = "Patrol";
		break;

	case EDMState::Suspect:
		StateString = "Suspect";
		break;

	case EDMState::Search:
		StateString = "Search";
		break;

	case EDMState::Chase:
		StateString = "Chase";
		break;

	case EDMState::Combat:
		StateString = "Combat";
		break;

	default:
		StateString = "Unknown";
		break;
	}

	GEngine->AddOnScreenDebugMessage(
		1,
		0.f,
		FColor::Green,
		FString::Printf(TEXT("State: %s"), *StateString));
}
