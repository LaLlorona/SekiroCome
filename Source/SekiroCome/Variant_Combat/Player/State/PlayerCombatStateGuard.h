// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCombatState.h"

#include "PlayerCombatStateGuard.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROCOME_API UPlayerCombatStateGuard : public UObject, public IPlayerCombatState
{
	GENERATED_BODY()

public:
	virtual void UpdateState(float deltaTime) override;
	virtual float GetElapsedTimeFromStateEnter() override;
};
