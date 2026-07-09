#include "CombatMontageTableFactory.h"

#if WITH_EDITOR

#include "Table/Table/CombatMontageTable.h"
#include "Table/Row/FCombatMontageRow.h"

UCombatMontageTableFactory::UCombatMontageTableFactory()
{
	SupportedClass = UCombatMontageTable::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UCombatMontageTableFactory::ConfigureProperties()
{
	// always use FCombatMontageRow, skip the row-structure picker dialog
	Struct = FCombatMontageRow::StaticStruct();
	return true;
}

FText UCombatMontageTableFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("Combat Montage Data Table"));
}

UDataTable* UCombatMontageTableFactory::MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags)
{
	// UDataTableFactory::MakeNewDataTable hardcodes NewObject<UDataTable>(...), ignoring SupportedClass,
	// so this must be overridden to actually construct our subclass.
	return NewObject<UCombatMontageTable>(InParent, Name, Flags);
}

#endif // WITH_EDITOR
