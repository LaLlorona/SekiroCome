#include "CombatEnemyDefenseDataTableFactory.h"

#if WITH_EDITOR

#include "CombatEnemyDefenseDataTable.h"
#include "FCombatEnemyDefenseRow.h"

UCombatEnemyDefenseDataTableFactory::UCombatEnemyDefenseDataTableFactory()
{
	SupportedClass = UCombatEnemyDefenseDataTable::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UCombatEnemyDefenseDataTableFactory::ConfigureProperties()
{
	// always use FCombatEnemyDefenseRow, skip the row-structure picker dialog
	Struct = FCombatEnemyDefenseRow::StaticStruct();
	return true;
}

FText UCombatEnemyDefenseDataTableFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("Combat Enemy Defense Data Table"));
}

UDataTable* UCombatEnemyDefenseDataTableFactory::MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags)
{
	// UDataTableFactory::MakeNewDataTable hardcodes NewObject<UDataTable>(...), ignoring SupportedClass,
	// so this must be overridden to actually construct our subclass.
	return NewObject<UCombatEnemyDefenseDataTable>(InParent, Name, Flags);
}

#endif // WITH_EDITOR
