#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FCombatEnemyDefenseRow.h"
#include "CombatEnemyDefenseDataTable.generated.h"

/** DataTable locked to FCombatEnemyDefenseRow, so any table of this class always uses that row schema */
UCLASS()
class SEKIROCOME_API UCombatEnemyDefenseDataTable : public UDataTable
{
	GENERATED_BODY()

public:
	UCombatEnemyDefenseDataTable();

	/** Finds a row by name, returns nullptr if not found */
	const FCombatEnemyDefenseRow* FindByRowName(FName RowName) const;
};
