#pragma once

#include "Engine/DataTable.h"
#include "FCombatWeaponDamageDataRow.generated.h"

/** Row key (RowName) identifies the weapon (e.g. weapon ID) */
USTRUCT(BlueprintType)
struct FCombatWeaponDamageDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponDamage")
	float ThrustDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponDamage")
	float SlashDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponDamage")
	float BluntDamage = 0.0f;
};
