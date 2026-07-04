#include "CombatDefenseDataTable.h"

UCombatDefenseDataTable::UCombatDefenseDataTable()
{
	RowStruct = FCombatArmorDataRow::StaticStruct();
}

const FCombatArmorDataRow& UCombatDefenseDataTable::FindByRowNameOrThrow(FName RowName) const
{
	const FString ContextString = FString::Printf(TEXT("Could Not Find Row Name %s"), *RowName.ToString());
	const FCombatArmorDataRow* Row = FindRow<FCombatArmorDataRow>(RowName, ContextString);
	checkf(Row, TEXT("UCombatDefenseDataTable '%s': row '%s' not found"), *GetName(), *RowName.ToString());
	return *Row;
}
