#include "CombatTuningDataTableFactory.h"

#if WITH_EDITOR

#include "CombatTuningDataTable.h"
#include "FCombatTuningRow.h"

UCombatTuningDataTableFactory::UCombatTuningDataTableFactory()
{
	SupportedClass = UCombatTuningDataTable::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UCombatTuningDataTableFactory::ConfigureProperties()
{
	// always use FCombatTuningRow, skip the row-structure picker dialog
	Struct = FCombatTuningRow::StaticStruct();
	return true;
}

FText UCombatTuningDataTableFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("Combat Tuning Data Table"));
}

UDataTable* UCombatTuningDataTableFactory::MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags)
{
	// UDataTableFactory::MakeNewDataTable hardcodes NewObject<UDataTable>(...), ignoring SupportedClass,
	// so this must be overridden to actually construct our subclass.
	return NewObject<UCombatTuningDataTable>(InParent, Name, Flags);
}

#endif // WITH_EDITOR
