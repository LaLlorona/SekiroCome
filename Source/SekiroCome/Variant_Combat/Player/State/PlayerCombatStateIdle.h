// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCombatState.h"

#include "PlayerCombatStateIdle.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROCOME_API UPlayerCombatStateIdle : public UObject, public IPlayerCombatState
{
	GENERATED_BODY()

public:
	virtual void UpdateState(float deltaTime) override;
	virtual float GetElapsedTimeFromStateEnter() override;
	virtual EAnimationStateEnum GetAnimationStateEnum() override;
};
