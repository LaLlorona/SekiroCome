#pragma once



#include "Engine/DataTable.h"
#include "CombatTypes.h"
#include "Table/Id/FMontageId.h"
#include "FCombatAttackInfoRow.generated.h"

USTRUCT(BlueprintType)
struct FCombatAttackInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MontageInfo")
	FMontageId MontageNameKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MontageInfo")
	FName MontageSectionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MontageInfo")
	float MontagePlaySpeed;

	/** Key used to look up the damage multiplier row in UCombatDamageMultiplierTable */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	EAttackType AttackDamageMultiplierKey;
};
