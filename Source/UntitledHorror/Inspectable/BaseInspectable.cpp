// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInspectable.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"
#include "UntitledHorror/Components/InteractableComponent.h"
#include "UntitledHorror/Character/MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseInspectable::ABaseInspectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	RootComponent = SceneRoot;

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetupAttachment(SceneRoot);

	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	PreviewMesh->SetupAttachment(SceneRoot);

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComp"));
	
	SceneCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComp"));
	SceneCaptureComp->SetupAttachment(SceneRoot);

	InteractableComp->OnInteract.AddDynamic(this, &ABaseInspectable::OnInteract);
}

// Called when the game starts or when spawned
void ABaseInspectable::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();

	PreviewMesh->SetHiddenInGame(true);
	PreviewMesh->SetRelativeLocation(FVector::ZeroVector);
	PreviewMesh->SetRelativeRotation(FRotator::ZeroRotator);
}

void ABaseInspectable::OnInteract(AActor* Interactor)
{
	AMainCharacter* Char = Cast<AMainCharacter>(Interactor);
	if(!Char)
	{
		UE_LOG(LogTemp, Warning, TEXT("In ABaseInspectable::OnInteract(AActor* Interactor) Char is not valid!"));
		return;
	}
	
	SetUpForInspect(Char);
	Char->Inspect(this);
}

// Called every frame
void ABaseInspectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseInspectable::SetUpForInspect(AActor* Interactor)
{
	PreviewMesh->SetHiddenInGame(false);
	MainMesh->SetHiddenInGame(true);

	FVector MeshLocation = PreviewMesh->Bounds.Origin;

	FVector Direction =
		(Interactor->GetActorLocation() - MeshLocation).GetSafeNormal();

	const float Distance = 50.0f;

	FVector CameraLocation =
		MeshLocation + Direction * Distance;

	SceneCaptureComp->SetWorldLocation(CameraLocation);

	SceneCaptureComp->SetWorldRotation(
		UKismetMathLibrary::FindLookAtRotation(
			CameraLocation,
			MeshLocation
		)
	);
}

void ABaseInspectable::SetUpForGame()
{
	PreviewMesh->SetHiddenInGame(true);
	PreviewMesh->SetRelativeRotation(PreviewRotation);
	MainMesh->SetHiddenInGame(false);
}

void ABaseInspectable::ZoomInOut(float Value)
{
	float FOV = SceneCaptureComp->FOVAngle + (Value * -6.0f);
	float NewFOV = FMath::Clamp(FOV, 30, 120);
	SceneCaptureComp->FOVAngle = NewFOV;
}

void ABaseInspectable::UpdateRotation(FVector2D Value)
{
	float NewX = Value.X * -5.0f;
	float NewY = Value.Y * -5.0f;

	FRotator NewRot = FRotator(NewY, 0.0f, NewX);
	FRotator Result = UKismetMathLibrary::ComposeRotators(PreviewMesh->GetRelativeRotation(), NewRot);
	PreviewMesh->SetRelativeRotation(Result);
}

