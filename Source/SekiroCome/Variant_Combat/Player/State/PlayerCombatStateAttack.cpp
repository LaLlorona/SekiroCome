// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateAttack.h"

#include "CombatCharacter.h"
#include "FCombatStateInitializeParameter.h"

void UPlayerCombatStateAttack::InitializeState(const FCombatStateInitializeParameter& Parameter)
{
	InitParam = Parameter;
}

void UPlayerCombatStateAttack::UpdateState(float deltaTime)
{
	ElapsedTimeFromStateEnter += deltaTime;
}

float UPlayerCombatStateAttack::GetElapsedTimeFromStateEnter()
{
	return ElapsedTimeFromStateEnter;
}

EAnimationStateEnum UPlayerCombatStateAttack::GetAnimationStateEnum()
{
	return EAnimationStateEnum::Normal;
}

EAttackDirection UPlayerCombatStateAttack::GetPreparedAttackDirection() const
{
	return InitParam.StateEnterAttackDirection;
}

bool UPlayerCombatStateAttack::IsStateExpired()
{
	return !InitParam.OwnerCharacter->IsAttacking();
}

void UPlayerCombatStateAttack::OnStateEnter()
{
	ElapsedTimeFromStateEnter = 0.0f;
}

void UPlayerCombatStateAttack::OnStateFinish()
{
}
