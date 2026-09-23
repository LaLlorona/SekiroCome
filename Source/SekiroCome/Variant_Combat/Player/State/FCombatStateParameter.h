#pragma once
#include "CoreMinimal.h"
#include "FCombatStateComponentInitializeParameter.h"
#include "Table/Id/FAttackInfoId.h"

#include "FCombatStateParameter.generated.h"


USTRUCT()
struct FCombatStateParameter
{
	GENERATED_BODY()

	UPROPERTY()
	FCombatStateComponentInitializeParameter StateComponentInitializeParameter;

	EAttackDirection StateEnterAttackDirection;

	// CombatAttackInfoTable lookup key, filled only when entering an Attack state. Empty (TOptional) when transitioning to any non-Attack State.
	TOptional<FAttackInfoId> AttackInfoRowName;

	static FCombatStateParameter CreateWithPreparedAttackDirection(
			  const FCombatStateComponentInitializeParameter& ComponentParameter);

	static FCombatStateParameter CreateForAttack(
			  const FCombatStateComponentInitializeParameter& ComponentParameter,
			  FAttackInfoId InAttackInfoRowName);

	FCombatStateParameter() = default;
	FCombatStateParameter(const FCombatStateComponentInitializeParameter& InStateComponentParameter, EAttackDirection InAttackDirection) :
	StateComponentInitializeParameter(InStateComponentParameter),
	StateEnterAttackDirection(InAttackDirection)
	{
	}
};
