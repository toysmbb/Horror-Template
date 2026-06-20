// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDoor.h"
#include "UntitledHorror/Components/InteractableComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundCue.h"
#include "UntitledHorror/Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseDoor::ABaseDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
	InteractableComp->OnInteract.AddDynamic(this, &ABaseDoor::OnInteract);
}

// Called when the game starts or when spawned
void ABaseDoor::BeginPlay()
{
	Super::BeginPlay();
	
	InitialRotation = MainMesh->GetRelativeRotation();

	if(CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindDynamic(this, &ABaseDoor::OpenDoor);
		TimeLine.AddInterpFloat(CurveFloat, TimeLineProgress);
	}
}

void ABaseDoor::OnInteract(AActor* Interactor)
{
	AMainCharacter* Char = Cast<AMainCharacter>(Interactor);
	if (!Char)
	{
		UE_LOG(LogTemp, Warning, TEXT("In ABaseDoor::SetToSameSide(AActor* Interactor) Char is not valid!"));
		return;
	}

	if (bIsCanOpen == false)
		return;

	if (bIsDoorClosed)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), DoorOpenSound);
		Char->MakeNoise();
		TimeLine.Play();
	}
	else
		TimeLine.Reverse();

	bIsDoorClosed = !bIsDoorClosed;
}

void ABaseDoor::OpenDoor(float Value)
{
	float DoorAngle = bIsDoorOnSameSide ? -DoorRotateAngle : DoorRotateAngle;

	FRotator NewRot = FRotator(InitialRotation.Pitch, InitialRotation.Yaw + DoorAngle * Value, InitialRotation.Roll);

	MainMesh->SetRelativeRotation(NewRot);
}

void ABaseDoor::SetToSameSide(AActor* Interactor)
{
	AMainCharacter* Char = Cast<AMainCharacter>(Interactor);
	if(!Char)
	{
		UE_LOG(LogTemp, Warning, TEXT("In ABaseDoor::SetToSameSide(AActor* Interactor) Char is not valid!"));
		return;
	}

	FVector CharForwardVec = Char->GetActorForwardVector();
	FVector DoorForwardVec = GetActorForwardVector();

	bIsDoorOnSameSide = FVector::DotProduct(CharForwardVec, DoorForwardVec) >= 0;
}

// Called every frame
void ABaseDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeLine.TickTimeline(DeltaTime);
}

