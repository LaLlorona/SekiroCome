#pragma once


#include "FCombatStateParameter.h"
#include "CombatCharacter.h"
#include "PlayerCombatStateMachineComponent.h"


FCombatStateParameter FCombatStateParameter::CreateWithPreparedAttackDirection(const FCombatStateComponentInitializeParameter& ComponentParameter)
{
	auto direction = ComponentParameter.OwnerCharacter->GetCombatStateComponent()->GetPreparedAttackDirection();
	return FCombatStateParameter(ComponentParameter, direction);
}

FCombatStateParameter FCombatStateParameter::CreateForAttack(const FCombatStateComponentInitializeParameter& ComponentParameter, FAttackInfoId InAttackInfoRowName)
{
	FCombatStateParameter Parameter = CreateWithPreparedAttackDirection(ComponentParameter);
	Parameter.AttackInfoRowName = InAttackInfoRowName;
	return Parameter;
}
