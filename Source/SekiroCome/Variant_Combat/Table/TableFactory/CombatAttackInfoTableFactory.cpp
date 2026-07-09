#include "CombatAttackInfoTableFactory.h"

#if WITH_EDITOR

#include "Table/Table/CombatAttackInfoTable.h"
#include "Table/Row/FCombatAttackInfoRow.h"

UCombatAttackInfoTableFactory::UCombatAttackInfoTableFactory()
{
	SupportedClass = UCombatAttackInfoTable::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UCombatAttackInfoTableFactory::ConfigureProperties()
{
	// always use FCombatAttackInfoRow, skip the row-structure picker dialog
	Struct = FCombatAttackInfoRow::StaticStruct();
	return true;
}

FText UCombatAttackInfoTableFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("Combat Attack Info Data Table"));
}

UDataTable* UCombatAttackInfoTableFactory::MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags)
{
	// UDataTableFactory::MakeNewDataTable hardcodes NewObject<UDataTable>(...), ignoring SupportedClass,
	// so this must be overridden to actually construct our subclass.
	return NewObject<UCombatAttackInfoTable>(InParent, Name, Flags);
}

#endif // WITH_EDITOR
