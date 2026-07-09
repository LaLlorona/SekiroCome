#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Table/Row/FCombatTuningRow.h"
#include "Table/Id/FCombatTuningId.h"
#include "CombatTuningDataTable.generated.h"

/** DataTable locked to FCombatTuningRow, so any table of this class always uses that row schema */
UCLASS()
class SEKIROCOME_API UCombatTuningDataTable : public UDataTable
{
	GENERATED_BODY()

public:
	UCombatTuningDataTable();

	/** Finds a row by CombatTuningId. Crashes (checkf) if the row does not exist - a missing row is a data-authoring bug, not a normal runtime case */
	const FCombatTuningRow& FindByCombatTuningId(FCombatTuningId CombatTuningId) const;
};
