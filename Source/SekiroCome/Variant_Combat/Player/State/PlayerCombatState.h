// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.h"
#include "UObject/Interface.h"

#include "PlayerCombatState.generated.h"

struct FCombatStateInitializeParameter;

// This class does not need to be modified.
UINTERFACE()
class UPlayerCombatState : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SEKIROCOME_API IPlayerCombatState
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void InitializeState(const FCombatStateInitializeParameter& Parameter) = 0;
	virtual void UpdateState(float deltaTime) = 0;
	virtual float GetElapsedTimeFromStateEnter() = 0;
	virtual EAnimationStateEnum GetAnimationStateEnum() = 0;
	virtual EAttackDirection GetPreparedAttackDirection() const = 0;
	virtual bool IsStateExpired() = 0;
	virtual void OnStateEnter() = 0;
	virtual void OnStateFinish() = 0;
	
};
