#pragma once


#include "FCombatStateParameter.h"
#include "CombatCharacter.h"
#include "PlayerCombatStateMachineComponent.h"


FCombatStateParameter FCombatStateParameter::CreateWithPreparedAttackDirection(const FCombatStateComponentInitializeParameter& ComponentParameter)
{
	auto direction = ComponentParameter.OwnerCharacter->GetCombatStateComponent()->GetPreparedAttackDirection();
	return FCombatStateParameter(ComponentParameter, direction);
}
