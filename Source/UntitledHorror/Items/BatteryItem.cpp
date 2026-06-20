// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryItem.h"
#include "UntitledHorror/Character/MainCharacter.h"

ABatteryItem::ABatteryItem()
{
	ItemInfo.ItemRef = this->GetClass();
}

void ABatteryItem::BeginPlay()
{
	Super::BeginPlay();
}

void ABatteryItem::OnInteract(AActor* Interactor)
{
	Super::OnInteract(Interactor);

	UE_LOG(LogTemp, Warning, TEXT("ABatteryItem::OnInteract(AActor* Interactor) is called"));
}

void ABatteryItem::OnUseItem(AActor* Interactor)
{
	Super::OnUseItem(Interactor);

	AMainCharacter* Char = Cast<AMainCharacter>(Interactor);
	if(!Char)
	{
		UE_LOG(LogTemp, Warning, TEXT("In ABatteryItem::OnUseItem(AActor* Interactor) Char is not valid!"));
		return;
	}

	Char->RemainingBatteryTime += 20.0f;
}

