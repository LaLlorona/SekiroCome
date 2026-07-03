#include "CombatWeaponDamageDataTableFactory.h"

#if WITH_EDITOR

#include "CombatWeaponDamageDataTable.h"
#include "FCombatWeaponDamageRow.h"

UCombatWeaponDamageDataTableFactory::UCombatWeaponDamageDataTableFactory()
{
	SupportedClass = UCombatWeaponDamageDataTable::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UCombatWeaponDamageDataTableFactory::ConfigureProperties()
{
	// always use FCombatWeaponDamageRow, skip the row-structure picker dialog
	Struct = FCombatWeaponDamageRow::StaticStruct();
	return true;
}

FText UCombatWeaponDamageDataTableFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("Combat Weapon Damage Data Table"));
}

UDataTable* UCombatWeaponDamageDataTableFactory::MakeNewDataTable(UObject* InParent, FName Name, EObjectFlags Flags)
{
	// UDataTableFactory::MakeNewDataTable hardcodes NewObject<UDataTable>(...), ignoring SupportedClass,
	// so this must be overridden to actually construct our subclass.
	return NewObject<UCombatWeaponDamageDataTable>(InParent, Name, Flags);
}

#endif // WITH_EDITOR
