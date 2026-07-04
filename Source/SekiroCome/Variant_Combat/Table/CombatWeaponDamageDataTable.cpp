#include "CombatWeaponDamageDataTable.h"

UCombatWeaponDamageDataTable::UCombatWeaponDamageDataTable()
{
	RowStruct = FCombatWeaponDamageDataRow::StaticStruct();
}

const FCombatWeaponDamageDataRow& UCombatWeaponDamageDataTable::FindByRowNameOrThrow(FName RowName) const
{
	const FString ContextString = FString::Printf(TEXT("Could Not Find Row Name %s"), *RowName.ToString());
	const FCombatWeaponDamageDataRow* Row = FindRow<FCombatWeaponDamageDataRow>(RowName, ContextString);
	checkf(Row, TEXT("UCombatWeaponDamageDataTable '%s': row '%s' not found"), *GetName(), *RowName.ToString());
	return *Row;
}
