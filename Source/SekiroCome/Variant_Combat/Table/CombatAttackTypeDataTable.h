#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FCombatAttackTypeRow.h"
#include "CombatAttackTypeDataTable.generated.h"

/** DataTable locked to FCombatAttackTypeRow, so any table of this class always uses that row schema */
UCLASS()
class SEKIROCOME_API UCombatAttackTypeDataTable : public UDataTable
{
	GENERATED_BODY()

public:
	UCombatAttackTypeDataTable();

	/** Finds a row by name, returns nullptr if not found */
	const FCombatAttackTypeRow* FindByRowName(FName RowName) const;
};
