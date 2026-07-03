#pragma once

#include "Engine/DataTable.h"
#include "FCombatAttackTypeRow.generated.h"

/**
 * Row key (RowName) identifies the attack type, e.g.:
 * "Left", "Right", "Up", "Down", "LeftRiposte", "RightRiposte", "UpRiposte", "DownRiposte",
 * "ComboFinisher", "MasterStrike"
 */
USTRUCT(BlueprintType)
struct FCombatAttackTypeRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AttackType")
	float ThrustMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AttackType")
	float SlashMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AttackType")
	float BluntMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AttackType", meta=(ClampMin="0.0", ClampMax="1.0"))
	float PriorityHealthDamageRatio = 0.0f;
};
