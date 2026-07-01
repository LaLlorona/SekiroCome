#include "CombatLogic.h"
#include "CombatLogic/FAttackData.h"
#include "Interfaces/CombatAttacker.h"
#include "Interfaces/CombatDamageable.h"
#include "Interfaces/CombatParryable.h"

  
namespace CombatLogic
{
	void ResolveAttack(AActor* AttackerActor, ICombatAttacker* Attacker, AActor* DamagedActor, ICombatDamageable* Damageable, const FAttackData& AttackData)
	{
		if (ICombatParryable* Parryable = Cast<ICombatParryable>(DamagedActor))
		{
			if (Parryable->CanParryNow())
			{
				if (ICombatDamageable* AttackerDamageable = Cast<ICombatDamageable>(AttackerActor))
				{
					//ToDo: Reposte Animation 재생
					AttackerDamageable->ApplyDamage(AttackData.Damage, AttackerActor, AttackData.DamageLocation, AttackData.DamageImpulse);
					/*Parryable->*/
					Parryable->ChangeToRiposteState();
					return;
				}
			}
		}
		Damageable->ApplyDamage(AttackData.Damage, AttackerActor, AttackData.DamageLocation, AttackData.DamageImpulse);
	}
}
