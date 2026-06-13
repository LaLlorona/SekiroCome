// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatManager.generated.h"

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
	void ResolveAttack(ICombatAttacker Attacker, ICombatDamageable Damageable);
};
