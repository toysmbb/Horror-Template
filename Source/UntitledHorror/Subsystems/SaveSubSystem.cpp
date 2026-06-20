// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSubSystem.h"
#include "UntitledHorror/SaveGameDefault.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void USaveSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SavedData = Cast<USaveGameDefault>(UGameplayStatics::CreateSaveGameObject(USaveGameDefault::StaticClass()));
}

void USaveSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

void USaveSubSystem::SaveGameData()
{
	SavedData = Cast<USaveGameDefault>(UGameplayStatics::CreateSaveGameObject(USaveGameDefault::StaticClass()));
	
	if (!SavedData) 
	{
		UE_LOG(LogTemp, Warning, TEXT("No data has saved!"));
		return;
	}

	if (UWorld* World = GetWorld())
	{
		SavedData->MapName = World->GetMapName();
		SavedData->MapName.RemoveFromStart(World->StreamingLevelsPrefix); 
	}

	OnSavedGame.Broadcast(SavedData);

	UGameplayStatics::SaveGameToSlot(SavedData, SlotName, 0);
	UE_LOG(LogTemp, Log, TEXT("Game Saved successfully to slot: %s"), *SlotName);
}


void USaveSubSystem::LoadGameData()
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Save file does not exist."));
		return;
	}

	SavedData = Cast<USaveGameDefault>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (!SavedData) return;

	bIsPendingLoad = true;

	UGameplayStatics::OpenLevel(GetWorld(), FName(*SavedData->MapName));
}


void USaveSubSystem::NotifyLevelLoaded()
{
	if (bIsPendingLoad && SavedData)
	{
		OnLoadedGame.Broadcast(SavedData);
		bIsPendingLoad = false;
	}
}
