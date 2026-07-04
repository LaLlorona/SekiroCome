#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CombatTypes.h"
#include "FCombatDamageMultiplierDataRow.h"
#include "CombatDamageMultiplierTable.generated.h"

/** DataTable locked to FCombatAttackTypeRow, so any table of this class always uses that row schema */
UCLASS()
class SEKIROCOME_API UCombatDamageMultiplierTable : public UDataTable
{
	GENERATED_BODY()

public:
	UCombatDamageMultiplierTable();

	/** Finds a row by attack type. RowNames in the table must match the EAttackType value names exactly. Crashes (checkf) if the row does not exist - a missing row is a data-authoring bug, not a normal runtime case */
	const FCombatDamageMultiplierDataRow& FindByAttackTypeOrThrow(EAttackType AttackType) const;

private:
	/** Converts the enum value to the FName the underlying DataTable is keyed by */
	static FName ToRowName(EAttackType AttackType);
};
