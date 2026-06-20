// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "BaseDoor.generated.h"

class UInteractableComponent;

UCLASS()
class UNTITLEDHORROR_API ABaseDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Interact")
	UInteractableComponent* InteractableComp;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* MainMesh;

	FTimeline TimeLine;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* DoorOpenSound;

	UPROPERTY(EditAnywhere, Category = "DoorAnimation")
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, Category = "DoorAnimation")
	float DoorRotateAngle = 90.0f;

	UPROPERTY()
	FRotator InitialRotation;

	bool bIsDoorClosed = true;
	bool bIsDoorOnSameSide;
public:
	UPROPERTY(EditAnywhere, Category = "Variables")
	bool bIsCanOpen = false;
private:
	UFUNCTION()
	virtual void OnInteract(AActor* Interactor);

	UFUNCTION()
	void OpenDoor(float Value);

	UFUNCTION()
	void SetToSameSide(AActor* Interactor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
