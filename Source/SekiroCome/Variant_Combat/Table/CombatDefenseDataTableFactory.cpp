#include "CombatDefenseDataTableFactory.h"

#if WITH_EDITOR

#include "CombatDefenseDataTable.h"
#include "FCombatArmorDataRow.h"

UCombatDefenseDataTableFactory::UCombatDefenseDataTableFactory()
{
	SupportedClass = UCombatDefenseDataTable::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UCombatDefenseDataTableFactory::ConfigureProperties()
{
	// always use FCombatDefenseRow, skip the row-structure picker dialog
	Struct = FCombatArmorDataRow::StaticStruct();
	return true;
}

FText UCombatDefenseDataTableFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("Combat Defense Data Table"));
}

UDataTable* UCombatDefenseDataTableFactory::MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags)
{
	// UDataTableFactory::MakeNewDataTable hardcodes NewObject<UDataTable>(...), ignoring SupportedClass,
	// so this must be overridden to actually construct our subclass.
	return NewObject<UCombatDefenseDataTable>(InParent, Name, Flags);
}

#endif // WITH_EDITOR
