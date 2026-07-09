#pragma once

#include "CoreMinimal.h"
#include "FWeaponId.generated.h"

/** Strongly-typed row key into UCombatWeaponDamageDataTable. Wraps FName so it can't be confused at compile time with another table's FName key (e.g. ArmorTypeID, MontageId). */
USTRUCT(BlueprintType)
struct FWeaponId
{
	GENERATED_BODY()

	FWeaponId() = default;
	explicit FWeaponId(FName InName) : Name(InName) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponId")
	FName Name;

	FName Get() const { return Name; }

	bool operator==(const FWeaponId& Other) const { return Name == Other.Name; }

	friend uint32 GetTypeHash(const FWeaponId& Id) { return GetTypeHash(Id.Name); }
};
