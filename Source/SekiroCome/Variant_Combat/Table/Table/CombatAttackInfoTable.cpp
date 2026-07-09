#include "CombatAttackInfoTable.h"

UCombatAttackInfoTable::UCombatAttackInfoTable()
{
	RowStruct = FCombatAttackInfoRow::StaticStruct();
}

const FCombatAttackInfoRow& UCombatAttackInfoTable::FindByAttackInfoId(FAttackInfoId AttackInfoId) const
{
	const FName RowName = AttackInfoId.Get();
	const FString ContextString = FString::Printf(TEXT("Could Not Find Row Name %s"), *RowName.ToString());
	const FCombatAttackInfoRow* Row = FindRow<FCombatAttackInfoRow>(RowName, ContextString);
	checkf(Row, TEXT("UCombatAttackInfoTable '%s': row '%s' not found"), *GetName(), *RowName.ToString());
	return *Row;
}
