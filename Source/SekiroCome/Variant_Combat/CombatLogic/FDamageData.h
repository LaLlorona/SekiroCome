#pragma once

#include "CoreMinimal.h"
#include "FDamageData.generated.h"

USTRUCT(BlueprintType)
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float PriorityHealthDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float RemainingDamage = 0.0f;

	FDamageData() = default;
	FDamageData(float InPriorityHealthDamage, float InRemainingDamage): PriorityHealthDamage(InPriorityHealthDamage), RemainingDamage(InRemainingDamage)
	{
	}
};
