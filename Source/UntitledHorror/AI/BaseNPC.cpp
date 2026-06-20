// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseNPC.h"
#include "UntitledHorror/Components/HealthComponent.h"
#include "UntitledHorror/Components/CombatComponent.h"
#include "UntitledHorror/Character/MainCharacter.h"
#include "AIC_DM.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ABaseNPC::ABaseNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HPComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HPComp"));
	HPComp->CurrentHP = 100;

	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
}

// Called when the game starts or when spawned
void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseNPC::ReduceHP(int32 Value)
{
	HPComp->ReduceHP(Value);
	if (HPComp->CurrentHP <= 0)
		Die();
}

void ABaseNPC::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("ABaseNPC::Die() is called"));
}

void ABaseNPC::TraceHit()
{
	CombatComp->TraceHit();
	UE_LOG(LogTemp, Warning, TEXT("ABaseNPC::TraceHit is called"));
}

