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

	// Attack 상태로 진입할 때만 채워지는 CombatAttackInfoTable 조회 키. Attack 이외의 State로 전환할 때는 비어있다(TOptional).
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
