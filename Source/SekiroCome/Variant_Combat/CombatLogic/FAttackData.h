#pragma once
#include "../CombatTypes.h"

struct FAttackData
{
	float Damage = 0.f;
	FVector DamageLocation = FVector::ZeroVector;
	FVector DamageImpulse = FVector::ZeroVector;
	EAttackDirection AttackDirection = EAttackDirection::Down;

	FAttackData() = default;
	FAttackData(float InDamage, const FVector& InLocation, const FVector& InImpulse, EAttackDirection InDirection): Damage(InDamage), DamageLocation(InLocation), DamageImpulse(InImpulse), AttackDirection(InDirection)
	{
	}
};
