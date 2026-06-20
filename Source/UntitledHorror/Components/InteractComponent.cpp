// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "InteractableComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractComponent::TraceInteract()
{
    APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController());
    if (!PC)
        return;

    FVector Start;
    FRotator Rotation;
    PC->GetPlayerViewPoint(Start, Rotation);
    FVector End = Start + (Rotation.Vector() * InteractRadius);
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetOwner());

    FCollisionShape Sphere = FCollisionShape::MakeSphere(50.0f);

    bool bIsHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, Sphere, Params);

    FColor TraceColor = bIsHit ? FColor::Green : FColor::Red;
    DrawDebugSphere(GetWorld(), End, 50.0f, 16, TraceColor, false, 2.0f);

    if (bIsHit)
    {
        if (AActor* ItemActor = HitResult.GetActor())
        {
            
            if (UInteractableComponent* ItemComp = ItemActor->FindComponentByClass<UInteractableComponent>())
                ItemComp->OnInteractDetected(GetOwner());
        }
    }
}

