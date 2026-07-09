#pragma once

#include "CoreMinimal.h"
#include "FArmorTypeId.generated.h"

/** Strongly-typed row key into UCombatDefenseDataTable. Wraps FName so it can't be confused at compile time with another table's FName key (e.g. WeaponId, MontageId). */
USTRUCT(BlueprintType)
struct FArmorTypeId
{
	GENERATED_BODY()

	FArmorTypeId() = default;
	explicit FArmorTypeId(FName InName) : Name(InName) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ArmorTypeId")
	FName Name;

	FName Get() const { return Name; }

	bool operator==(const FArmorTypeId& Other) const { return Name == Other.Name; }

	friend uint32 GetTypeHash(const FArmorTypeId& Id) { return GetTypeHash(Id.Name); }
};
