// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatManager.generated.h"

struct FAttackData;
class ICombatAttacker;
class ICombatDamageable;
/**
 * 
 */
UCLASS()
class SEKIROCOME_API UCombatManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void ResolveAttack(AActor* AttackerActor, ICombatAttacker* Attacker, AActor* DamagedActor, ICombatDamageable* Damageable, const FAttackData& AttackData);
};
