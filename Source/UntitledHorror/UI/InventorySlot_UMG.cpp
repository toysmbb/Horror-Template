// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot_UMG.h"
#include "Framework/Application/SlateApplication.h"

void UInventorySlot_UMG::SetupSlot(FGameItemInfo SlotInfo)
{
	ItemInfo = SlotInfo;

	DropItem->SetVisibility(ESlateVisibility::Hidden);
	UseItem->SetVisibility(ESlateVisibility::Hidden);

	if (SlotIcon)
	{
		if (SlotInfo.Icon)
		{
			SlotIcon->SetBrushFromTexture(SlotInfo.Icon);
			SlotIcon->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			SlotIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (Quantity)
	{
		if (SlotInfo.ItemRef && SlotInfo.Quantity > 1)
		{
			Quantity->SetText(FText::AsNumber(SlotInfo.Quantity));
			Quantity->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Quantity->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

FReply UInventorySlot_UMG::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		FVector2D MouseScreenPosition = InMouseEvent.GetScreenSpacePosition();
		FGeometry SlotIconGeometry = SlotIcon->GetCachedGeometry();
		if (SlotIconGeometry.IsUnderLocation(MouseScreenPosition))
		{
			DropItem->SetVisibility(ESlateVisibility::Visible);
			if (ItemInfo.bIsUseable)
				UseItem->SetVisibility(ESlateVisibility::Visible);
			else
				UseItem->SetVisibility(ESlateVisibility::Hidden);

			return FReply::Handled();
		}
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		FVector2D MouseScreenPosition = InMouseEvent.GetScreenSpacePosition();
		FGeometry DropItemGeometry = DropItem->GetCachedGeometry();
		FGeometry UseItemGeometry = UseItem->GetCachedGeometry();

		if (DropItemGeometry.IsUnderLocation(MouseScreenPosition))
		{
			UE_LOG(LogTemp, Warning, TEXT("Left clicked at DropItem menu!"));
			
			OnItemDropped.Broadcast(ItemInfo);
			return FReply::Handled();
		}
		else if (UseItem->IsVisible() && UseItemGeometry.IsUnderLocation(MouseScreenPosition))
		{
			UE_LOG(LogTemp, Warning, TEXT("Left clicked at UseItem menu!"));
		
			OnItemUsed.Broadcast(ItemInfo);
			return FReply::Handled();
		}
		else
		{
			DropItem->SetVisibility(ESlateVisibility::Hidden);
			UseItem->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	return FReply::Unhandled();;
}
