// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatMontageSet.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROCOME_API UCombatMontageSet : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* RiposteMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* PartialParryMontage;
public:
	UAnimMontage* GetRiposteMontage() const { return RiposteMontage; }
	UAnimMontage* GetPartialParryMontage() const { return PartialParryMontage; }
};
