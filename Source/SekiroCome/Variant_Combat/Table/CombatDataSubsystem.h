// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CombatDataSubsystem.generated.h"


/**
 * 
 */

class UCombatDataTableManager;

UCLASS()
class SEKIROCOME_API UCombatDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	UPROPERTY()
	
	TObjectPtr<UCombatDataTableManager> InGameTableManager;

public:
	UCombatDataTableManager* GetInGameTableManager() { return InGameTableManager.Get(); }
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	static UCombatDataSubsystem* GetCombatDataSubsystem(const UObject* WorldContextObject);
};
