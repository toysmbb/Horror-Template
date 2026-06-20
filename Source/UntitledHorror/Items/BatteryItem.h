// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BatteryItem.generated.h"


UCLASS()
class UNTITLEDHORROR_API ABatteryItem : public ABaseItem
{
	GENERATED_BODY()
protected:
	ABatteryItem();

	virtual void BeginPlay() override;

	virtual void OnInteract(AActor* Interactor) override;

public:
	virtual void OnUseItem(AActor* Interactor) override;
};
