// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCombatState.h"

#include "PlayerCombatStatePartialParry.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROCOME_API UPlayerCombatStatePartialParry : public UObject, public IPlayerCombatState 
{
	GENERATED_BODY()

public:
	virtual void InitializeState(const FCombatStateInitializeParameter& Parameter) override;
	virtual void UpdateState(float deltaTime) override;
	virtual float GetElapsedTimeFromStateEnter() override;
	virtual EAnimationStateEnum GetAnimationStateEnum() override;
	virtual bool IsStateExpired() override;
	virtual void OnStateEnter() override;
	virtual void OnStateFinish() override;
};
