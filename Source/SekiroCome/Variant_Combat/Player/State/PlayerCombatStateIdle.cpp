// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateIdle.h"

#include "CombatTypes.h"
#include "FCombatStateInitializeParameter.h"

void UPlayerCombatStateIdle::InitializeState(const FCombatStateInitializeParameter& Parameter)
{
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
