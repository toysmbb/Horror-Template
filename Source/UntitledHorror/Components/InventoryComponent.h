// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UntitledHorror/UI/Inventory_UMG.h"
#include "UntitledHorror/Items/GameItemInfo.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNTITLEDHORROR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Items")
	TArray<FGameItemInfo> ItemsArray;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UInventory_UMG> Inventory_UMGBase;

	UPROPERTY()
	UInventory_UMG* Inventory_UMG;

	UPROPERTY(EditAnywhere, Category = "Info")
	int32 MaxSize = 24;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool AddItem(FGameItemInfo& ItemInfo);
	
	UFUNCTION()
	void DropItem(FGameItemInfo ItemInfo);

	UFUNCTION()
	void UseItem(FGameItemInfo ItemInfo);

	void ConsumeItem(FGameItemInfo ItemInfo);

	void HideInventory();
	void MakeVisible();

	void ConstructInventory();

	UInventory_UMG* GetInventoryWidget() { return Inventory_UMG; }
};
