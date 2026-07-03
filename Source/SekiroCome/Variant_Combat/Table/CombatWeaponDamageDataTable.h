#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FCombatWeaponDamageRow.h"
#include "CombatWeaponDamageDataTable.generated.h"

/** DataTable locked to FCombatWeaponDamageRow, so any table of this class always uses that row schema */
UCLASS()
class SEKIROCOME_API UCombatWeaponDamageDataTable : public UDataTable
{
	GENERATED_BODY()

public:
	UCombatWeaponDamageDataTable();

	/** Finds a row by name, returns nullptr if not found */
	const FCombatWeaponDamageRow* FindByRowName(FName RowName) const;
};
