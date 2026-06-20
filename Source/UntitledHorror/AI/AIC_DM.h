// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_AIController.h"
#include "EDMState.h"
#include "AIC_DM.generated.h"

UCLASS()
class UNTITLEDHORROR_API AAIC_DM : public ABase_AIController
{
	GENERATED_BODY()
protected:

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
public:
	AAIC_DM();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Variables")
	FVector LastLocation;

	UPROPERTY(EditAnywhere, Category = "State")
	EDMState CurrentState = EDMState::Patrol;
};
