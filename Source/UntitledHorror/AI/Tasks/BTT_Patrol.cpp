// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Patrol.h"
#include "UntitledHorror/AI/Base_AIController.h"
#include "UntitledHorror/AI/BaseNPC.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Patrol::UBTT_Patrol()
{
	NodeName = TEXT("Patrol");
}

EBTNodeResult::Type UBTT_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABase_AIController* AIC = Cast<ABase_AIController>(OwnerComp.GetOwner());
	if (!AIC)
		return EBTNodeResult::Failed;
    
	UBlackboardComponent* BB = Cast<UBlackboardComponent>(AIC->GetBlackboardComponent());
	ABaseNPC* NPC = Cast<ABaseNPC>(AIC->GetPawn());
	if (!NPC || !BB)
		return EBTNodeResult::Failed;

	TArray<AActor*> PatrolPoints = NPC->GetSplineArray();
	
	if (PatrolPoints.Num() == 0)
		return EBTNodeResult::Failed;

	int32 pointslength = PatrolPoints.Num();

	if (pointsindex == pointslength)
		pointsindex = 0;

	FVector PointLocation = PatrolPoints[pointsindex]->GetActorLocation();
	BB->SetValueAsVector(FName("PatrolLocation"), PointLocation);
	pointsindex++;
	return EBTNodeResult::Succeeded;
}
