#include "CombatAttackTypeDataTable.h"

UCombatAttackTypeDataTable::UCombatAttackTypeDataTable()
{
	RowStruct = FCombatAttackTypeRow::StaticStruct();
}

const FCombatAttackTypeRow* UCombatAttackTypeDataTable::FindByRowName(FName RowName) const
{
	const FString ContextString = FString::Printf(TEXT("Could Not Find Row Name %s"), *RowName.ToString());
	return FindRow<FCombatAttackTypeRow>(RowName, ContextString);
}
