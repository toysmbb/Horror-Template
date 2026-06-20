// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Base_AIController.generated.h"

class UBehaviorTree;

UCLASS()
class UNTITLEDHORROR_API ABase_AIController : public AAIController
{
	GENERATED_BODY()
public:
	ABase_AIController();
protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, Category = "Perception")
	UAIPerceptionComponent* AIPercepComp;

	UPROPERTY(EditAnywhere, Category = "Perception")
	UAISenseConfig_Sight* SenseSight;

	UPROPERTY(EditAnywhere, Category = "Perception")
	UAISenseConfig_Hearing* SenseHearing;

	UPROPERTY(EditAnywhere, Category = "BT")
	UBehaviorTree* BT;
};
