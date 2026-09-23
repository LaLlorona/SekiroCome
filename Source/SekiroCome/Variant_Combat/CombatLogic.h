#pragma once

#include "CoreMinimal.h"
#include "Table/Id/FAttackInfoId.h"

enum class EAttackDirection : uint8;
struct FAttackData;
class ICombatAttacker;
class ICombatDamageable;

namespace CombatLogic
{
	void ResolveAttack(AActor* AttackerActor, ICombatAttacker* Attacker, AActor* DamagedActor, ICombatDamageable* Damageable, const FAttackData& AttackData);

	EAttackDirection GetNextAttackDirectionOnAttack(EAttackDirection CurrentAttackDirection);

	EAttackDirection GetNextAttackDirectionOnBlock(EAttackDirection BlockedAttackDirection);

	/** Default CombatAttackInfoTable RowName used when entering a basic (non-combo) attack. Not used for cases such as combos, where the RowName can't be determined from AttackDirection alone. */
	FAttackInfoId GetAttackInfoRowNameByDirection(const EAttackDirection AttackDirection);
}
