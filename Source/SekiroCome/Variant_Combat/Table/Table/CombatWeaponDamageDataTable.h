#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Table/Row/FCombatWeaponDamageDataRow.h"
#include "Table/Id/FWeaponId.h"
#include "CombatWeaponDamageDataTable.generated.h"

/** DataTable locked to FCombatWeaponDamageRow, so any table of this class always uses that row schema */
UCLASS()
class SEKIROCOME_API UCombatWeaponDamageDataTable : public UDataTable
{
	GENERATED_BODY()

public:
	UCombatWeaponDamageDataTable();

	/** Finds a row by WeaponId. Crashes (checkf) if the row does not exist - a missing row is a data-authoring bug, not a normal runtime case */
	const FCombatWeaponDamageDataRow& FindByWeaponId(FWeaponId WeaponId) const;
};
