// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDHORROR_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(Editanywhere, Category="Variables")
	int32 CurrentHP;

	void ReduceHP(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "GetterFuncs")
	int32 GetCurrentHP() { return CurrentHP; }

	UFUNCTION(BlueprintCallable, Category = "SetterFuncs")
	void SetCurrentHP(int32 NewHP) { CurrentHP = NewHP; }
};
