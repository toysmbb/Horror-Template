// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseInspectable.generated.h"

class UInteractableComponent;
class USceneCaptureComponent2D;
class UStaticMeshComponent;

UCLASS()
class UNTITLEDHORROR_API ABaseInspectable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseInspectable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MainMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* PreviewMesh;

	UPROPERTY(EditAnywhere, Category = "Interact")
	UInteractableComponent* InteractableComp;

	UFUNCTION()
	virtual void OnInteract(AActor* Interactor);


public:	
	UPROPERTY(EditAnywhere, Category = "SceneCapture")
	USceneCaptureComponent2D* SceneCaptureComp;

	FVector StartLocation;
	FRotator StartRotation;
	FVector PreviewLocation;
	FRotator PreviewRotation;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetStartLocation() { return StartLocation; }

	FRotator GetStartRotation() { return StartRotation; }

	void SetUpForInspect(AActor* Interactor);
	void SetUpForGame();

	void ZoomInOut(float Value);
	void UpdateRotation(FVector2D Value);
};
