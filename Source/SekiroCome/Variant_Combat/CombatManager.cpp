// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatManager.h"

#include "CombatAttacker.h"
#include "CombatDamageable.h"
#include "CombatParryable.h"
#include "CombatLogic/FAttackData.h"

void UCombatManager::ResolveAttack(AActor* AttackerActor, ICombatAttacker* Attacker, AActor* DamagedActor, ICombatDamageable* Damageable,
                                   const FAttackData& AttackData)
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
				return;
			}
		}
	}
	Damageable->ApplyDamage(AttackData.Damage, AttackerActor, AttackData.DamageLocation, AttackData.DamageImpulse);
}
