// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateIdle.h"

#include "CombatCharacter.h"
#include "CombatLogic.h"
#include "CombatTypes.h"
#include "FCombatStateParameter.h"
#include "PlayerCombatStateAttack.h"
#include "PlayerCombatStateMachineComponent.h"

void UPlayerCombatStateIdle::InitializeState(const FCombatStateParameter& Parameter)
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
	auto combatStateMachineComponent = InitParam.StateComponentInitializeParameter.OwnerCharacter->GetCombatStateComponent();
	auto directionOnAttackInputPressed = combatStateMachineComponent->GetPreparedAttackDirection();
	auto attackStateParam = FCombatStateParameter::CreateForAttack(
		InitParam.StateComponentInitializeParameter,
		CombatLogic::GetAttackInfoRowNameByDirection(directionOnAttackInputPressed));
	PendingNextState = CreateCombatState<UPlayerCombatStateAttack>(attackStateParam);
}
