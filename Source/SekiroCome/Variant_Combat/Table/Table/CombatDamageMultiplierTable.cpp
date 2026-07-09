#include "CombatDamageMultiplierTable.h"

UCombatDamageMultiplierTable::UCombatDamageMultiplierTable()
{
	RowStruct = FCombatDamageMultiplierDataRow::StaticStruct();
}

const FCombatDamageMultiplierDataRow& UCombatDamageMultiplierTable::FindByAttackTypeOrThrow(EAttackType AttackType) const
{
	const FName RowName = ToRowName(AttackType);
	const FString ContextString = FString::Printf(TEXT("Could Not Find Row Name %s"), *RowName.ToString());
	const FCombatDamageMultiplierDataRow* Row = FindRow<FCombatDamageMultiplierDataRow>(RowName, ContextString);
	checkf(Row, TEXT("UCombatDamageMultiplierTable '%s': row '%s' not found"), *GetName(), *RowName.ToString());
	return *Row;
}

FName UCombatDamageMultiplierTable::ToRowName(EAttackType AttackType)
{
	return FName(*StaticEnum<EAttackType>()->GetNameStringByValue(static_cast<int64>(AttackType)));
}
