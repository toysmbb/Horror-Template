// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameItemInfo.h"
#include "BaseKey.generated.h"

class ABaseDoor;
class UInteractableComponent;

UCLASS()
class UNTITLEDHORROR_API ABaseKey : public AActor
{
	GENERATED_BODY()
protected:
	ABaseKey();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnInteract(AActor* Interactor);

	void SetDoorOpen();

	UPROPERTY(EditAnywhere, Category = "Door")
	ABaseDoor* Door = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	UInteractableComponent* InteractableComp;

	UPROPERTY(EditAnywhere, Category = "Info")
	FGameItemInfo ItemInfo;
};
