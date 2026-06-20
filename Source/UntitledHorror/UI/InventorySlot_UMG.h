// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"
#include "UntitledHorror/Items/GameItemInfo.h"
#include "Components/TextBlock.h"
#include "InventorySlot_UMG.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDropItem, FGameItemInfo, ItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUseItem, FGameItemInfo, ItemInfo);

UCLASS()
class UNTITLEDHORROR_API UInventorySlot_UMG : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDropItem OnItemDropped;

	UPROPERTY(BlueprintAssignable)
	FOnUseItem OnItemUsed;

	UPROPERTY()
	FGameItemInfo ItemInfo;

	UPROPERTY(meta = (BindWidget))
	UImage* SlotIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* DropItem;

	UPROPERTY(meta = (BindWidget))
	UImage* UseItem;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* Quantity;

	void SetupSlot(FGameItemInfo SlotInfo);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
