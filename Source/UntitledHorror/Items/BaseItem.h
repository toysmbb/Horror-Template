// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameItemInfo.h"
#include "BaseItem.generated.h"

class UInteractableComponent;

UCLASS()
class UNTITLEDHORROR_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	UInteractableComponent* InteractableComp;

	UPROPERTY(EditAnywhere, Category = "Info")
	FGameItemInfo ItemInfo;

	UFUNCTION()
	virtual void OnInteract(AActor* Interactor);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	virtual void OnUseItem(AActor* Interactor);
};
