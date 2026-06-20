// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameDefault.generated.h"


USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	FRotator Rotation = FRotator::ZeroRotator;
};

UCLASS()
class UNTITLEDHORROR_API USaveGameDefault : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
	FPlayerInfo PlayerInfo;
};
