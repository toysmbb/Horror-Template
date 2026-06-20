// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSubSystem.generated.h"

class USaveGameDefault;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSavedGame, USaveGameDefault*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLoadedGame, USaveGameDefault*);


UCLASS()
class UNTITLEDHORROR_API USaveSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Saving")
	void SaveGameData();

	UFUNCTION(BlueprintCallable, Category = "Loaing")
	void LoadGameData();

	UFUNCTION(BlueprintPure, Category = "Saving")
	USaveGameDefault* GetSavedData() { return SavedData; }

	UFUNCTION(BlueprintCallable, Category = "Saving")
	void NotifyLevelLoaded();

	FOnSavedGame OnSavedGame;
	FOnLoadedGame OnLoadedGame;
protected:
	UPROPERTY(EditAnywhere, Category = "Data")
	USaveGameDefault* SavedData = nullptr;

	FString SlotName = TEXT("UntitledHorrorSlot");

	bool bIsPendingLoad = false;
};
