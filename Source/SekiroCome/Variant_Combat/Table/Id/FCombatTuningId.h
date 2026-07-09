#pragma once

#include "CoreMinimal.h"
#include "FCombatTuningId.generated.h"

/** Strongly-typed row key into UCombatTuningDataTable. Wraps FName so it can't be confused at compile time with another table's FName key (e.g. WeaponId, ArmorTypeId). */
USTRUCT(BlueprintType)
struct FCombatTuningId
{
	GENERATED_BODY()

	FCombatTuningId() = default;
	explicit FCombatTuningId(FName InName) : Name(InName) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CombatTuningId")
	FName Name;

	FName Get() const { return Name; }

	bool operator==(const FCombatTuningId& Other) const { return Name == Other.Name; }

	friend uint32 GetTypeHash(const FCombatTuningId& Id) { return GetTypeHash(Id.Name); }
};
