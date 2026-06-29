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

UENUM(BlueprintType)
enum class EAnimationStateEnum : uint8
{
	Normal,
	Guard,
	PartialParry,
	Riposte,
};

UENUM(BlueprintType)
enum class ECombatStateEnum : uint8
{
	Idle,
	Guard,
	PartialParry,
	PerfectParryRiposte,
};

UENUM(BlueprintType)
enum class ECombatInputDirectionEnum : uint8
{
	NoInput,
	Left,
	Top,
	Right,
	Down
};