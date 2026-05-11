#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.generated.h"

UENUM(BlueprintType)
enum class EAttackDirection : uint8
{
	Up,
	Down,
	Right,
	Left
};

UENUM(BlueprintType)
enum class EParryResult : uint8
{
	PerfectParry,
	PartialParry,
	Block
};