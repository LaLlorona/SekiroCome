// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateIdle.h"

#include "CombatCharacter.h"
#include "CombatTypes.h"
#include "FCombatStateInitializeParameter.h"
#include "PlayerCombatStateAttack.h"

void UPlayerCombatStateIdle::InitializeState(const FCombatStateInitializeParameter& Parameter)
{
	InitParam = Parameter;
}

void UPlayerCombatStateIdle::UpdateState(float deltaTime)
{
}

float UPlayerCombatStateIdle::GetElapsedTimeFromStateEnter()
{
	return 0;
}

EAnimationStateEnum UPlayerCombatStateIdle::GetAnimationStateEnum()
{
	return EAnimationStateEnum::Normal;
}

TOptional<TScriptInterface<IPlayerCombatState>> UPlayerCombatStateIdle::GetStateToTransition()
{
	if (PendingNextState.GetObject() != nullptr)
	{
		return PendingNextState;
	}
	return {};
}

void UPlayerCombatStateIdle::OnStateEnter()
{
}

void UPlayerCombatStateIdle::OnStateFinish()
{
}

void UPlayerCombatStateIdle::OnAttackInputPressed()
{
	PendingNextState = NewObject<UPlayerCombatStateAttack>();
}
