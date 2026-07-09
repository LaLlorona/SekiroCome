#pragma once

#include "CoreMinimal.h"
#include "FMontageId.generated.h"

/** Strongly-typed row key into UCombatMontageTable. Wraps FName so it can't be confused at compile time with another table's FName key (e.g. WeaponID, ArmorTypeID). */
USTRUCT(BlueprintType)
struct FMontageId
{
	GENERATED_BODY()

	FMontageId() = default;
	explicit FMontageId(FName InName) : Name(InName) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MontageId")
	FName Name;

	FName Get() const { return Name; }

	bool operator==(const FMontageId& Other) const { return Name == Other.Name; }

	friend uint32 GetTypeHash(const FMontageId& Id) { return GetTypeHash(Id.Name); }
};
