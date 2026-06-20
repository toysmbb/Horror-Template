// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "UntitledHorror/Items/BaseItem.h"
#include "UntitledHorror/Character/MainCharacter.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddItem(FGameItemInfo& ItemInfo)
{
	for (FGameItemInfo& ExistingItem : ItemsArray)
	{
		if (ExistingItem == ItemInfo)
		{
			if (ExistingItem.Quantity < ExistingItem.MaxStack)
			{
				ExistingItem.Quantity += ItemInfo.Quantity;
				ExistingItem.Quantity = FMath::Min(ExistingItem.Quantity, ExistingItem.MaxStack);

				if (Inventory_UMG)
					Inventory_UMG->UpdateInventory(ItemsArray);

				return true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Stack is full!"));
				return false;
			}
		}
	}

	if (ItemsArray.Num() >= MaxSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full!"));
		return false;
	}

	ItemsArray.Add(ItemInfo);

	if (Inventory_UMG)
		Inventory_UMG->UpdateInventory(ItemsArray);

	return true;
}

void UInventoryComponent::DropItem(FGameItemInfo ItemInfo)
{
	AMainCharacter* Char = Cast<AMainCharacter>(GetOwner());

	if (!Char)
	{
		UE_LOG(LogTemp, Warning, TEXT("Char is invalid!"));
		return;
	}

	if (!ItemInfo.ItemRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemRef is invalid!"));
		return;
	}

	FVector Forward = Char->GetActorForwardVector();
	FVector ItemSpawnLocation = Char->GetActorLocation() + Forward * 150.0f;

	FActorSpawnParameters Params;
	Params.Owner = Char;
	Params.Instigator = Char;

	ABaseItem* Item = GetWorld()->SpawnActor<ABaseItem>(ItemInfo.ItemRef, ItemSpawnLocation, FRotator::ZeroRotator, Params);

	if (!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn item!"));
		return;
	}

	ConsumeItem(ItemInfo);
}

void UInventoryComponent::UseItem(FGameItemInfo ItemInfo)
{
	if (!ItemInfo.bIsUseable) return;

	if (!ItemInfo.ItemRef)
		return;

	ABaseItem* CDO = ItemInfo.ItemRef->GetDefaultObject<ABaseItem>();
	if (!CDO)
		return;

	CDO->OnUseItem(GetOwner());

	ConsumeItem(ItemInfo);
}

void UInventoryComponent::ConsumeItem(FGameItemInfo ItemInfo)
{
	int32 FoundIndex = INDEX_NONE;

	for (int32 i = 0; i < ItemsArray.Num(); i++)
	{
		if (ItemsArray[i] == ItemInfo)
		{
			FoundIndex = i;
			break;
		}
	}

	if (FoundIndex == INDEX_NONE) return;

	ItemsArray[FoundIndex].Quantity--;

	if (ItemsArray[FoundIndex].Quantity <= 0)
		ItemsArray.RemoveAt(FoundIndex);

	if (Inventory_UMG)
		Inventory_UMG->UpdateInventory(ItemsArray);
}

void UInventoryComponent::HideInventory()
{
	if (Inventory_UMG)
		Inventory_UMG->SetVisibility(ESlateVisibility::Hidden);
	else
		UE_LOG(LogTemp, Warning, TEXT("In UInventoryComponent::HideInventory() Inventory_UMG is not valid!"));

}

void UInventoryComponent::MakeVisible()
{
	if (Inventory_UMG)
	{
		Inventory_UMG->UpdateInventory(ItemsArray);
		Inventory_UMG->SetVisibility(ESlateVisibility::Visible);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("In UInventoryComponent::MakeVisible() Inventory_UMG is not valid!"));
}

void UInventoryComponent::ConstructInventory()
{
	AMainCharacter* Char = Cast<AMainCharacter>(GetOwner());
	if(!Char)
	{
		UE_LOG(LogTemp, Warning, TEXT("In UInventoryComponent::ConstructInventory() Char is not valid"));
		return;
	}
	APlayerController* PC = Cast<APlayerController>(Char->GetController());
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("In UInventoryComponent::ConstructInventory() PC is not valid"));
		return;
	}

	Inventory_UMG = CreateWidget<UInventory_UMG>(PC, Inventory_UMGBase);
	if (!Inventory_UMG)
		UE_LOG(LogTemp, Warning, TEXT("In UInventoryComponent::ConstructInventory() Inventory_UMG is not valid!"));
	
	Inventory_UMG->AddToViewport();
	Inventory_UMG->OnInventoryItemDropped.AddDynamic(this, &UInventoryComponent::DropItem);
	Inventory_UMG->OnInventoryItemUsed.AddDynamic(this, &UInventoryComponent::UseItem);
	Inventory_UMG->SetVisibility(ESlateVisibility::Hidden);
}
