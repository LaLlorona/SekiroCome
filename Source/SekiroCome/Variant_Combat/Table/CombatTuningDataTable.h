#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FCombatTuningRow.h"
#include "CombatTuningDataTable.generated.h"

/** DataTable locked to FCombatTuningRow, so any table of this class always uses that row schema */
UCLASS()
class SEKIROCOME_API UCombatTuningDataTable : public UDataTable
{
	GENERATED_BODY()

public:
	UCombatTuningDataTable();

	/** Finds a row by name. Crashes (checkf) if the row does not exist - a missing row is a data-authoring bug, not a normal runtime case */
	const FCombatTuningRow& FindByRowNameOrThrow(FName RowName) const;
};
