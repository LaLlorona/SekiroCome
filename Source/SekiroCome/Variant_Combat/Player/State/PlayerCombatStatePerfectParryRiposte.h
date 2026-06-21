// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCombatState.h"

#include "PlayerCombatStatePerfectParryRiposte.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROCOME_API UPlayerCombatStatePerfectParryRiposte : public UObject, public IPlayerCombatState
{
	GENERATED_BODY()
	float ElapsedTimeFromStateEnter;

public:
	virtual void UpdateState(float deltaTime) override;
	virtual float GetElapsedTimeFromStateEnter() override;
	virtual EAnimationStateEnum GetAnimationStateEnum() override;
};
