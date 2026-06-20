// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "UntitledHorror/Character/MainCharacter.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::TraceHit()
{
	UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::TraceHit is called!"));
	ACharacter* NPC = Cast<ACharacter>(GetOwner());
	if (!NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("NPC is nullptr!"));
		return;
	}
	FVector Start = NPC->GetActorLocation();
	FVector End = NPC->GetActorLocation() + NPC->GetActorForwardVector() * 150.0f;
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	FCollisionShape Sphere = FCollisionShape::MakeSphere(50.0f);

	bool bIsHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_Pawn, Sphere, Params);
	
	FColor TraceColor = bIsHit ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), Start, 50.0f, 16, TraceColor, false, 2.0f);
	DrawDebugSphere(GetWorld(), End, 50.0f, 16, TraceColor, false, 2.0f);
	DrawDebugLine(GetWorld(), Start, End, TraceColor, false, 2.0f, 0, 2.0f);

	if(bIsHit)
	{
		AMainCharacter* HitActor = Cast<AMainCharacter>(HitResult.GetActor());
		if(HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor is exist!"));
			HitActor->ReduceHP(Damage);
		}
	}
}

