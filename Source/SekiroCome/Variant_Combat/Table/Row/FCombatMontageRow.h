#pragma once

#include "Engine/DataTable.h"
#include "FCombatMontageRow.generated.h"

class UAnimMontage;

/**
 * Row key (RowName) identifies the montage (e.g. "Attack1", "AttackUp").
 * FCombatAttackInfoRow::MontageNameKey references this table's RowName.
 */
USTRUCT(BlueprintType)
struct FCombatMontageRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MontageInfo")
	TObjectPtr<UAnimMontage> Montage;
};
