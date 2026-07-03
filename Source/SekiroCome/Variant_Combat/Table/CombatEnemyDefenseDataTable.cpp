#include "CombatEnemyDefenseDataTable.h"

UCombatEnemyDefenseDataTable::UCombatEnemyDefenseDataTable()
{
	RowStruct = FCombatEnemyDefenseRow::StaticStruct();
}

const FCombatEnemyDefenseRow* UCombatEnemyDefenseDataTable::FindByRowName(FName RowName) const
{
	const FString ContextString = FString::Printf(TEXT("Could Not Find Row Name %s"), *RowName.ToString());
	return FindRow<FCombatEnemyDefenseRow>(RowName, ContextString);
}
