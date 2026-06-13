// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerCombatState.generated.h"

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
	virtual void UpdateState(float deltaTime);
	virtual float GetElapsedTimeFromStateEnter();
};
