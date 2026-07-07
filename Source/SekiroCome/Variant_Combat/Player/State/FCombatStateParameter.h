#pragma once
#include "CoreMinimal.h"
#include "FCombatStateComponentInitializeParameter.h"

#include "FCombatStateParameter.generated.h"


USTRUCT()
struct FCombatStateParameter
{
	GENERATED_BODY()

	UPROPERTY()
	FCombatStateComponentInitializeParameter StateComponentInitializeParameter;

	EAttackDirection StateEnterAttackDirection;

	static FCombatStateParameter CreateWithPreparedAttackDirection(
			  const FCombatStateComponentInitializeParameter& ComponentParameter);

	FCombatStateParameter() = default;
	FCombatStateParameter(const FCombatStateComponentInitializeParameter& InStateComponentParameter, EAttackDirection InAttackDirection) :
	StateComponentInitializeParameter(InStateComponentParameter),
	StateEnterAttackDirection(InAttackDirection)
	{
	}
};
