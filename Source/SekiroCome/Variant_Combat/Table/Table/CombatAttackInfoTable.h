#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Table/Row/FCombatAttackInfoRow.h"
#include "Table/Id/FAttackInfoId.h"
#include "CombatAttackInfoTable.generated.h"

/** DataTable locked to FCombatAttackInfoRow, so any table of this class always uses that row schema */
UCLASS()
class SEKIROCOME_API UCombatAttackInfoTable : public UDataTable
{
	GENERATED_BODY()

public:
	UCombatAttackInfoTable();

	/** Finds a row by AttackInfoId. Crashes (checkf) if the row does not exist - a missing row is a data-authoring bug, not a normal runtime case */
	const FCombatAttackInfoRow& FindByAttackInfoId(FAttackInfoId AttackInfoId) const;
};
