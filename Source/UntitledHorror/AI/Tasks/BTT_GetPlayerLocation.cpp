// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GetPlayerLocation.h"
#include "UntitledHorror/AI/Base_AIController.h"
#include "UntitledHorror/AI/BaseNPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_GetPlayerLocation::UBTT_GetPlayerLocation()
{
	NodeName = TEXT("Get Player Location");
}

EBTNodeResult::Type UBTT_GetPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABase_AIController* AIC = Cast<ABase_AIController>(OwnerComp.GetOwner());
	if (!AIC)
		return EBTNodeResult::Failed;

	UBlackboardComponent* BB = Cast<UBlackboardComponent>(AIC->GetBlackboardComponent());
	if (!BB)
		return EBTNodeResult::Failed;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
		return EBTNodeResult::Failed;

	FVector TargetLocation = PlayerPawn->GetActorLocation();
	BB->SetValueAsObject(FName("Target"), PlayerPawn);
	
	return EBTNodeResult::Succeeded;
}
