#include "CombatTuningDataTable.h"

UCombatTuningDataTable::UCombatTuningDataTable()
{
	RowStruct = FCombatTuningRow::StaticStruct();
}

const FCombatTuningRow* UCombatTuningDataTable::FindByRowName(FName RowName) const
{
	const FString ContextString = FString::Printf(TEXT("Could Not Find Row Name %s"), *RowName.ToString());
	return FindRow<FCombatTuningRow>(RowName, ContextString);
}
