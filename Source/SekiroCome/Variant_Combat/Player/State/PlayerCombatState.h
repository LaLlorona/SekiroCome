// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.h"
#include "FCombatStateParameter.h"
#include "UObject/Interface.h"

#include "PlayerCombatState.generated.h"

class IPlayerCombatState;
struct FCombatStateParameter;

// This class does not need to be modified.
UINTERFACE()
class UPlayerCombatState : public UInterface
{
	GENERATED_BODY()
};


class SEKIROCOME_API IPlayerCombatState
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void InitializeState(const FCombatStateParameter& Parameter) = 0;
	virtual void UpdateState(float deltaTime) = 0;
	virtual float GetElapsedTimeFromStateEnter() = 0;
	virtual EAnimationStateEnum GetAnimationStateEnum() = 0;
	virtual void OnStateEnter() = 0;
	virtual void OnStateFinish() = 0;

	//AttackInput 버퍼링을 할 수도 있으니, AttackInput 이 눌렸을 때 어떤 행동을 할지 결정합니다.
	virtual void OnAttackInputPressed() = 0;
	virtual TOptional<TScriptInterface<IPlayerCombatState>> GetStateToTransition() = 0;
};
template<typename T>
TScriptInterface<IPlayerCombatState> CreateCombatState(const FCombatStateParameter& Parameter)
{
	T* NewState = NewObject<T>();
	NewState->InitializeState(Parameter);
	return NewState;
}
