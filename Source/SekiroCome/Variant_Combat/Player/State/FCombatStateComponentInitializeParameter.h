#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.h"

class ACombatCharacter;
class UCombatMontageSet;


#include "FCombatStateComponentInitializeParameter.generated.h"


 

USTRUCT()
struct FCombatStateComponentInitializeParameter
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UCombatMontageSet> CombatMontageSet;


	UPROPERTY()
	TObjectPtr<ACombatCharacter> OwnerCharacter;

	FCombatStateComponentInitializeParameter() = default;
	FCombatStateComponentInitializeParameter(UCombatMontageSet* InCombatMontageSet, ACombatCharacter* InCombatCharacter):
	CombatMontageSet (InCombatMontageSet),
	OwnerCharacter(InCombatCharacter)
	{
	}
	
};

/*struct FAttackData
{
	float Damage = 0.f;
	FVector DamageLocation = FVector::ZeroVector;
	FVector DamageImpulse = FVector::ZeroVector;
	EAttackDirection AttackDirection = EAttackDirection::Down;

	FAttackData() = default;
	FAttackData(float InDamage, const FVector& InLocation, const FVector& InImpulse, EAttackDirection InDirection): Damage(InDamage), DamageLocation(InLocation), DamageImpulse(InImpulse), AttackDirection(InDirection)
	{
	}
};*/