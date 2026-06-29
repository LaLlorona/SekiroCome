#pragma once

#include "CoreMinimal.h"

struct FAttackData;
class ICombatAttacker;
class ICombatDamageable;

namespace CombatLogic
{
	void ResolveAttack(AActor* AttackerActor, ICombatAttacker* Attacker, AActor* DamagedActor, ICombatDamageable* Damageable, const FAttackData& AttackData);
}
