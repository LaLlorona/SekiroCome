#include "CombatMontageTable.h"

UCombatMontageTable::UCombatMontageTable()
{
	RowStruct = FCombatMontageRow::StaticStruct();
}

const FCombatMontageRow& UCombatMontageTable::FindByMontageId(FMontageId MontageId) const
{
	const FName RowName = MontageId.Get();
	const FString ContextString = FString::Printf(TEXT("Could Not Find Row Name %s"), *RowName.ToString());
	const FCombatMontageRow* Row = FindRow<FCombatMontageRow>(RowName, ContextString);
	checkf(Row, TEXT("UCombatMontageTable '%s': row '%s' not found"), *GetName(), *RowName.ToString());
	return *Row;
}
