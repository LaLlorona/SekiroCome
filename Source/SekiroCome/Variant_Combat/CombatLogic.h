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

	/** Basic(non-combo) attack이 진입할 때 사용할 CombatAttackInfoTable RowName의 기본값. AttackDirection만으로 RowName이 정해지지 않는 콤보 등의 경우에는 쓰지 않는다. */
	FAttackInfoId GetAttackInfoRowNameByDirection(const EAttackDirection AttackDirection);
}
