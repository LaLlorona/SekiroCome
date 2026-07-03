#pragma once

#include "Engine/DataTable.h"
#include "FCombatEnemyDefenseRow.generated.h"

/** Row key (RowName) identifies the enemy type (e.g. enemy type ID) */
USTRUCT(BlueprintType)
struct FCombatEnemyDefenseRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EnemyDefense")
	float ThrustDefense = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EnemyDefense")
	float SlashDefense = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EnemyDefense")
	float BluntDefense = 0.0f;
};
