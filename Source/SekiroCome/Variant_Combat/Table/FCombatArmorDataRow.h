#pragma once

#include "Engine/DataTable.h"
#include "FCombatArmorDataRow.generated.h"

/** Row key (RowName) identifies the armor type (e.g. armor type ID). Shared by both player characters and enemies. */
USTRUCT(BlueprintType)
struct FCombatArmorDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Defense")
	float ThrustDefense = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Defense")
	float SlashDefense = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Defense")
	float BluntDefense = 0.0f;
};
