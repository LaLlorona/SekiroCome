// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateIdle.h"

#include "CombatCharacter.h"
#include "CombatTypes.h"
#include "FCombatStateInitializeParameter.h"

void UPlayerCombatStateIdle::InitializeState(const FCombatStateInitializeParameter& Parameter)
{
	InitParam = Parameter;
}

void UPlayerCombatStateIdle::UpdateState(float deltaTime)
{
	EAttackDirection MoveDirection;
	if (InitParam.OwnerCharacter->GetMoveAttackDirection(MoveDirection))
	{
		InitParam.StateEnterAttackDirection = MoveDirection;
	}
}

float UPlayerCombatStateIdle::GetElapsedTimeFromStateEnter()
{
	return 0;
}

EAnimationStateEnum UPlayerCombatStateIdle::GetAnimationStateEnum()
{
	return EAnimationStateEnum::Normal;
}

EAttackDirection UPlayerCombatStateIdle::GetPreparedAttackDirection() const
{
	return InitParam.StateEnterAttackDirection;
}

bool UPlayerCombatStateIdle::IsStateExpired()
{
	return false;
}

void UPlayerCombatStateIdle::OnStateEnter()
{
}

void UPlayerCombatStateIdle::OnStateFinish()
{
}
