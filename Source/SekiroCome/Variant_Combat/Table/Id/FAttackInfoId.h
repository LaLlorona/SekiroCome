#pragma once

#include "CoreMinimal.h"
#include "FAttackInfoId.generated.h"

/** Strongly-typed row key into UCombatAttackInfoTable. Wraps FName so it can't be confused at compile time with another table's FName key (e.g. MontageId, WeaponId). */
USTRUCT(BlueprintType)
struct FAttackInfoId
{
	GENERATED_BODY()

	FAttackInfoId() = default;
	explicit FAttackInfoId(FName InName) : Name(InName) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AttackInfoId")
	FName Name;

	FName Get() const { return Name; }

	bool operator==(const FAttackInfoId& Other) const { return Name == Other.Name; }

	friend uint32 GetTypeHash(const FAttackInfoId& Id) { return GetTypeHash(Id.Name); }
};
