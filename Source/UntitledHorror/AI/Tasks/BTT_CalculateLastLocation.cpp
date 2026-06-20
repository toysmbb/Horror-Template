// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CalculateLastLocation.h"
#include "UntitledHorror/AI/AIC_DM.h"
#include "UntitledHorror/AI/BaseNPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTT_CalculateLastLocation::UBTT_CalculateLastLocation()
{
	NodeName = TEXT("Calculate last player location");
}

EBTNodeResult::Type UBTT_CalculateLastLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIC_DM* AIC = Cast<AAIC_DM>(OwnerComp.GetOwner());
	if (!AIC)
		return EBTNodeResult::Failed;

	UBlackboardComponent* BB = Cast<UBlackboardComponent>(AIC->GetBlackboardComponent());
	if (!BB)
		return EBTNodeResult::Failed;

	FVector LastLocation = AIC->LastLocation;
	LastLocation = FVector((FMath::FRandRange(LastLocation.X, LastLocation.X + 300.0f)),
		(FMath::FRandRange(LastLocation.Y, LastLocation.Y + 150.0f)), LastLocation.Z);

	BB->SetValueAsVector(KeyName, LastLocation);
	return EBTNodeResult::Succeeded;
}
