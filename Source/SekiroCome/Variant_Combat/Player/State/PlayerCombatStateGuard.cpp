// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateGuard.h"

#include "CombatCharacter.h"
#include "CombatTypes.h"
#include "CombatLogic/CombatDefine.h"
#include "FCombatStateParameter.h"

void UPlayerCombatStateGuard::InitializeState(const FCombatStateParameter& Parameter)
{
	InitParam = Parameter;
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

TOptional<TScriptInterface<IPlayerCombatState>> UPlayerCombatStateGuard::GetStateToTransition()
{
	return {};
}

void UPlayerCombatStateGuard::OnStateEnter()
{
	ElapsedTimeFromStateEnter = 0.0f;
}

void UPlayerCombatStateGuard::OnStateFinish()
{
}

void UPlayerCombatStateGuard::OnAttackInputPressed()
{
}

