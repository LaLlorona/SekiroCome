// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatParryable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCombatParryable : public UInterface
{
	GENERATED_BODY()
public:

};

/**
 * 
 */
class SEKIROCOME_API ICombatParryable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CanParryNow() const = 0;
	virtual void ChangeToRiposteState() = 0;
};
