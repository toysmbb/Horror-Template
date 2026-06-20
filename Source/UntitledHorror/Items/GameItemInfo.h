#pragma once
#include "CoreMinimal.h"
#include "GameItemInfo.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FGameItemInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	FName ItemName = "None";

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
    int32 Quantity = 1;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	int32 MaxStack = 10;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	bool bIsUseable = false;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	bool bIsEquipable = false;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	TSubclassOf<AActor> ItemRef;

	bool operator==(const FGameItemInfo& Other) const
	{
		return ItemRef == Other.ItemRef;
	}
};