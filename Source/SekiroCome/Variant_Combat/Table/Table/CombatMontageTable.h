#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Table/Row/FCombatMontageRow.h"
#include "Table/Id/FMontageId.h"
#include "CombatMontageTable.generated.h"

/** DataTable locked to FCombatMontageRow, so any table of this class always uses that row schema */
UCLASS()
class SEKIROCOME_API UCombatMontageTable : public UDataTable
{
	GENERATED_BODY()

public:
	UCombatMontageTable();

	/** Finds a row by MontageId. Crashes (checkf) if the row does not exist - a missing row is a data-authoring bug, not a normal runtime case */
	const FCombatMontageRow& FindByMontageId(FMontageId MontageId) const;
};
