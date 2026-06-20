// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseInteractableActor.generated.h"

class UInteractableComponent;

UCLASS()
class UNTITLEDHORROR_API ABaseInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Interactable")
	UInteractableComponent* InteractableComp;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* DropSound;

public:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* MainMesh;
protected:
	UFUNCTION()
	void OnInteract(AActor* Interactor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USoundCue* GetSound() { return DropSound; }
};
