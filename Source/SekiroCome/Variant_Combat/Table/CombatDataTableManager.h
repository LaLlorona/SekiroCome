// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "CombatDataTableManager.generated.h"

class UCombatDefenseDataTable;
class UCombatDamageMultiplierTable;
class UCombatWeaponDamageDataTable;
class UCombatTuningDataTable;
/**
 * 
 */
UCLASS()
class SEKIROCOME_API UCombatDataTableManager : public UDataAsset
{
	GENERATED_BODY()


	

public:
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UCombatTuningDataTable> CombatTuningDataTable;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UCombatWeaponDamageDataTable> WeaponDamageTable;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UCombatDamageMultiplierTable> AttackTypeTable;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UCombatDefenseDataTable> DefenseTable;
};

