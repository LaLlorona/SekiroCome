#include "CombatAttackTypeDataTableFactory.h"

#if WITH_EDITOR

#include "CombatAttackTypeDataTable.h"
#include "FCombatAttackTypeRow.h"

UCombatAttackTypeDataTableFactory::UCombatAttackTypeDataTableFactory()
{
	SupportedClass = UCombatAttackTypeDataTable::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UCombatAttackTypeDataTableFactory::ConfigureProperties()
{
	// always use FCombatAttackTypeRow, skip the row-structure picker dialog
	Struct = FCombatAttackTypeRow::StaticStruct();
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
	return NewObject<UCombatAttackTypeDataTable>(InParent, Name, Flags);
}

#endif // WITH_EDITOR
