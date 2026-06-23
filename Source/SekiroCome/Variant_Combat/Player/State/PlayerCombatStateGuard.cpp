// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateGuard.h"

#include "CombatTypes.h"
#include "CombatLogic/CombatDefine.h"
#include "FCombatStateInitializeParameter.h"

void UPlayerCombatStateGuard::InitializeState(const FCombatStateInitializeParameter& Parameter)
{
}

void UPlayerCombatStateGuard::UpdateState(float deltaTime)
{
	ElapsedTimeFromStateEnter += deltaTime;
}

float UPlayerCombatStateGuard::GetElapsedTimeFromStateEnter()
{
	return ElapsedTimeFromStateEnter;
}

EAnimationStateEnum UPlayerCombatStateGuard::GetAnimationStateEnum()
{
	return EAnimationStateEnum::Guard;
}

bool UPlayerCombatStateGuard::CanParryNow() const
{
	return ElapsedTimeFromStateEnter <= CombatDefine::RiposteMinimumTimeWindow;
}

bool UPlayerCombatStateGuard::IsStateExpired()
{
	return false;
}

void UPlayerCombatStateGuard::OnStateEnter()
{
	ElapsedTimeFromStateEnter = 0.0f;
}

void UPlayerCombatStateGuard::OnStateFinish()
{
}

