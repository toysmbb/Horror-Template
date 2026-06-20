// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_CalculateLastLocation.generated.h"



UCLASS()
class UNTITLEDHORROR_API UBTT_CalculateLastLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_CalculateLastLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Key")
	FName KeyName;
};
