// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStatePartialParry.h"

#include "FCombatStateInitializeParameter.h"

void UPlayerCombatStatePartialParry::InitializeState(const FCombatStateInitializeParameter& Parameter)
{
}

void UPlayerCombatStatePartialParry::UpdateState(float deltaTime)
{
}

float UPlayerCombatStatePartialParry::GetElapsedTimeFromStateEnter()
{
	return 0;
}

EAnimationStateEnum UPlayerCombatStatePartialParry::GetAnimationStateEnum()
{
	return EAnimationStateEnum::PartialParry;
}

bool UPlayerCombatStatePartialParry::IsStateExpired()
{
	return false;
}

void UPlayerCombatStatePartialParry::OnStateEnter()
{
}

void UPlayerCombatStatePartialParry::OnStateFinish()
{
}
