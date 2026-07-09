#pragma once

#include "Engine/DataTable.h"
#include "FCombatTuningRow.generated.h"
USTRUCT(BlueprintType)
struct FCombatTuningRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina")
	float SP_RegenPerSecond = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina")
	float SP_RegenDelayInSecond = 1.5f;

	/** How long the character stays in the Hit (stagger) state after taking damage, unable to start a new attack */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Hit")
	float HitStunDurationInSecond = 0.5f;

};
