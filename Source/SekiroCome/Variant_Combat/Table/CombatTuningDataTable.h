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

	/** Finds a row by name, returns nullptr if not found */
	const FCombatTuningRow* FindByRowName(FName RowName) const;
};
