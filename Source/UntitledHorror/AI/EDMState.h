#pragma once
#include "CoreMinimal.h"
#include "EDMState.generated.h"

UENUM(BlueprintType)
enum class EDMState : uint8
{
	Patrol UMETA(DisplayName = "Patrol"),
	Suspect UMETA(DisplayName = "Suspect"),
	Search UMETA(DisplayName = "Search"),
	Chase UMETA(DisplayName = "Chase"),
	Combat UMETA(DisplayName = "Combat")
};