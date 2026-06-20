// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "BaseNPC.generated.h"

class UHealthComponent;
class UCombatComponent;

UCLASS()
class UNTITLEDHORROR_API ABaseNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Patrol path")
	TArray<AActor*> SplinePoints;

	UPROPERTY(EditAnywhere, Category = "Health")
	UHealthComponent* HPComp;

	UPROPERTY(EditAnywhere, Category = "Health")
	UCombatComponent* CombatComp;
private:
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	TArray<AActor*> GetSplineArray() { return SplinePoints; }

	void ReduceHP(int32 Value);

	void Die();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TraceHit();
};
