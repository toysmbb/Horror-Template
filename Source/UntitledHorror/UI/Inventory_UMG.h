// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "InventorySlot_UMG.h"
#include "UntitledHorror/Items/GameItemInfo.h"
#include "Inventory_UMG.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemDropped, FGameItemInfo, ItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemUsed, FGameItemInfo, ItemInfo);

UCLASS()
class UNTITLEDHORROR_API UInventory_UMG : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* Inventory_Grid;

	UPROPERTY(EditAnywhere, Category = "Slot")
	TSubclassOf<UInventorySlot_UMG> SlotWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Slot")
	int32 MaxColumns = 5;

	UPROPERTY(EditAnywhere, Category = "Slot")
	int32 TotalSlots = 25;
public:
	void UpdateInventory(const TArray<FGameItemInfo>& Items);

	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemDropped OnInventoryItemDropped;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemUsed OnInventoryItemUsed;

	UFUNCTION()
	void HandleSlotItemDropped(FGameItemInfo ItemInfo);

	UFUNCTION()
	void HandleSlotItemUsed(FGameItemInfo ItemInfo);
};
