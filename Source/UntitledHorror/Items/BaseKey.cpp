// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseKey.h"
#include "UntitledHorror/Components/InteractableComponent.h"
#include "UntitledHorror/Interactable/BaseDoor.h"
#include "UntitledHorror/Components/InventoryComponent.h"

ABaseKey::ABaseKey()
{
	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComp"));

	ItemInfo.ItemRef = this->GetClass();
}

void ABaseKey::BeginPlay()
{
	Super::BeginPlay();

	InteractableComp->OnInteract.AddDynamic(this, &ABaseKey::OnInteract);
}

void ABaseKey::OnInteract(AActor* Interactor)
{
	SetDoorOpen();
	Destroy();
}


void ABaseKey::SetDoorOpen()
{
	if(!Door)
	{
		UE_LOG(LogTemp, Warning, TEXT("In ABaseKey::SetDoorOpen() Door is nullptr"));
		return;
	}

	Door->bIsCanOpen = true;
}
