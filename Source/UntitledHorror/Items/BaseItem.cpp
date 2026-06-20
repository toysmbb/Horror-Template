// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "UntitledHorror/Components/InteractableComponent.h"
#include "UntitledHorror/Components/InventoryComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComp"));

	ItemInfo.ItemRef = this->GetClass();
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	InteractableComp->OnInteract.AddDynamic(this, &ABaseItem::OnInteract);
	
}

void ABaseItem::OnInteract(AActor* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseItem::OnInteract() is called"));
	if(!Interactor)
	{
		UE_LOG(LogTemp, Warning, TEXT("In ABaseItem::OnInteract() Interactor is not valid!"));
		return;
	}
	UInventoryComponent* InvComp = Interactor->FindComponentByClass<UInventoryComponent>();
	if(InvComp)
	{
		if (InvComp->AddItem(ItemInfo))
			Destroy();
	}
}

void ABaseItem::OnUseItem(AActor* Interactor)
{
}


// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}