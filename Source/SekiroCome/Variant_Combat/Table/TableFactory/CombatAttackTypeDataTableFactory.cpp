#include "CombatAttackTypeDataTableFactory.h"

#if WITH_EDITOR

#include "Table/Table/CombatDamageMultiplierTable.h"
#include "Table/Row/FCombatDamageMultiplierDataRow.h"

UCombatAttackTypeDataTableFactory::UCombatAttackTypeDataTableFactory()
{
	SupportedClass = UCombatDamageMultiplierTable::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UCombatAttackTypeDataTableFactory::ConfigureProperties()
{
	// always use FCombatAttackTypeRow, skip the row-structure picker dialog
	Struct = FCombatDamageMultiplierDataRow::StaticStruct();
	return true;
}

FText UCombatAttackTypeDataTableFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("Combat Attack Type Data Table"));
}

UDataTable* UCombatAttackTypeDataTableFactory::MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags)
{
	// UDataTableFactory::MakeNewDataTable hardcodes NewObject<UDataTable>(...), ignoring SupportedClass,
	// so this must be overridden to actually construct our subclass.
	return NewObject<UCombatDamageMultiplierTable>(InParent, Name, Flags);
}

#endif // WITH_EDITOR
