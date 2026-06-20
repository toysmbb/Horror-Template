// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory_UMG.h"

void UInventory_UMG::UpdateInventory(const TArray<FGameItemInfo>& Items)
{
    if (!Inventory_Grid || !SlotWidgetClass) return;
    Inventory_Grid->ClearChildren();

    for (int32 i = 0; i < TotalSlots; ++i)
    {
        UInventorySlot_UMG* NewSlot = CreateWidget<UInventorySlot_UMG>(this, SlotWidgetClass);
        NewSlot->OnItemDropped.AddDynamic(this, &UInventory_UMG::HandleSlotItemDropped);
        NewSlot->OnItemUsed.AddDynamic(this, &UInventory_UMG::HandleSlotItemUsed);
        if (NewSlot)
        {
            if (Items.IsValidIndex(i))
            {
                NewSlot->SetupSlot(Items[i]);
            }
            else
            {
                NewSlot->SetupSlot(FGameItemInfo());
            }

            int32 Row = i / MaxColumns;
            int32 Column = i % MaxColumns;

            Inventory_Grid->AddChildToUniformGrid(NewSlot, Row, Column);
        }
    }
}

void UInventory_UMG::HandleSlotItemDropped(FGameItemInfo ItemInfo)
{
    OnInventoryItemDropped.Broadcast(ItemInfo);
}

void UInventory_UMG::HandleSlotItemUsed(FGameItemInfo ItemInfo)
{
    UE_LOG(LogTemp, Warning, TEXT("UInventory_UMG::HandleSlotItemUsed(FGameItemInfo ItemInfo) is called!"));
    OnInventoryItemUsed.Broadcast(ItemInfo);
}
