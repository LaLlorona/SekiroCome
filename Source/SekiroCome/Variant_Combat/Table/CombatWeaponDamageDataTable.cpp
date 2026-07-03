#include "CombatWeaponDamageDataTable.h"

UCombatWeaponDamageDataTable::UCombatWeaponDamageDataTable()
{
	RowStruct = FCombatWeaponDamageRow::StaticStruct();
}

const FCombatWeaponDamageRow* UCombatWeaponDamageDataTable::FindByRowName(FName RowName) const
{
	const FString ContextString = FString::Printf(TEXT("Could Not Find Row Name %s"), *RowName.ToString());
	return FindRow<FCombatWeaponDamageRow>(RowName, ContextString);
}
