#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.h"

class ACombatCharacter;
class UCombatMontageSet;


#include "FCombatStateInitializeParameter.generated.h"


 

USTRUCT()
struct FCombatStateInitializeParameter
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UCombatMontageSet> CombatMontageSet;


	UPROPERTY()
	TObjectPtr<ACombatCharacter> OwnerCharacter;

	EAttackDirection StateEnterAttackDirection = EAttackDirection::Down;
	
	FCombatStateInitializeParameter() = default;
	FCombatStateInitializeParameter(UCombatMontageSet* InCombatMontageSet, ACombatCharacter* InCombatCharacter, EAttackDirection InAttackDirection):  CombatMontageSet (InCombatMontageSet), OwnerCharacter(InCombatCharacter), StateEnterAttackDirection(InAttackDirection)
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