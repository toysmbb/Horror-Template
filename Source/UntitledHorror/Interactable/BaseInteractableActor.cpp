// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractableActor.h"
#include "Components/StaticMeshComponent.h"
#include "UntitledHorror/Components/InteractableComponent.h"
#include "UntitledHorror/Character/MainCharacter.h"

// Sets default values
ABaseInteractableActor::ABaseInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComp"));

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetSimulatePhysics(true);

	RootComponent = MainMesh;
}

// Called when the game starts or when spawned
void ABaseInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableComp->OnInteract.AddDynamic(this, &ABaseInteractableActor::OnInteract);
}

void ABaseInteractableActor::OnInteract(AActor* Interactor)
{
	AMainCharacter* Char = Cast<AMainCharacter>(Interactor);
	if(!Char)
	{
		UE_LOG(LogTemp, Warning, TEXT("In ABaseInteractableActor::OnInteract(AActor* Interactor) Char is not valid!"));
		return;
	}
	Char->AttachActor(this);
}

// Called every frame
void ABaseInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

