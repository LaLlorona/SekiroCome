#pragma once

#include "CoreMinimal.h"
#include "Montage/AttackAnimationSectionInfo.h"

enum class EAttackDirection : uint8;
struct FAttackData;
class ICombatAttacker;
class ICombatDamageable;
class FName;

namespace CombatLogic
{
	void ResolveAttack(AActor* AttackerActor, ICombatAttacker* Attacker, AActor* DamagedActor, ICombatDamageable* Damageable, const FAttackData& AttackData);

	EAttackDirection GetNextAttackDirectionOnAttack(EAttackDirection CurrentAttackDirection);

	EAttackDirection GetNextAttackDirectionOnBlock(EAttackDirection BlockedAttackDirection);

	AttackAnimationSectionInfo GetAnimationSectionNameByAttackDirection(const EAttackDirection AttackDirection);
}
