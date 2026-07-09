#include "CombatTuningDataTable.h"

UCombatTuningDataTable::UCombatTuningDataTable()
{
	RowStruct = FCombatTuningRow::StaticStruct();
}

const FCombatTuningRow& UCombatTuningDataTable::FindByCombatTuningId(FCombatTuningId CombatTuningId) const
{
	const FName RowName = CombatTuningId.Get();
	const FString ContextString = FString::Printf(TEXT("Could Not Find Row Name %s"), *RowName.ToString());
	const FCombatTuningRow* Row = FindRow<FCombatTuningRow>(RowName, ContextString);
	checkf(Row, TEXT("UCombatTuningDataTable '%s': row '%s' not found"), *GetName(), *RowName.ToString());
	return *Row;
}
