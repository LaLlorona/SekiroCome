#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Table/Row/FCombatArmorDataRow.h"
#include "Table/Id/FArmorTypeId.h"
#include "CombatDefenseDataTable.generated.h"

/** DataTable locked to FCombatDefenseRow, so any table of this class always uses that row schema */
UCLASS()
class SEKIROCOME_API UCombatDefenseDataTable : public UDataTable
{
	GENERATED_BODY()

public:
	UCombatDefenseDataTable();

	/** Finds a row by ArmorTypeId. Crashes (checkf) if the row does not exist - a missing row is a data-authoring bug, not a normal runtime case */
	const FCombatArmorDataRow& FindByArmorTypeId(FArmorTypeId ArmorTypeId) const;
};
