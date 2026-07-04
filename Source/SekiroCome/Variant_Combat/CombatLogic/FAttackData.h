#pragma once
#include "../CombatTypes.h"

struct FAttackData
{
	FVector DamageLocation = FVector::ZeroVector;
	FVector DamageImpulse = FVector::ZeroVector;
	EAttackDirection AttackDirection = EAttackDirection::Down;

	/** This attack's kind (e.g. Left/Right/Up/Down, Riposte, ComboFinisher, MasterStrike). Used as the row key into UCombatAttackTypeDataTable. */
	EAttackType AttackType = EAttackType::Left;

	FAttackData() = default;
	FAttackData(const FVector& InLocation, const FVector& InImpulse, const EAttackDirection InDirection, const EAttackType InAttackType): DamageLocation(InLocation), DamageImpulse(InImpulse), AttackDirection(InDirection), AttackType(InAttackType)
	{
	}
};
