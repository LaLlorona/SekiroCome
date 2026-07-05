// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStatePartialParry.h"

#include "CombatCharacter.h"
#include "FCombatStateInitializeParameter.h"
#include "Montage/CombatMontageSet.h"

void UPlayerCombatStatePartialParry::InitializeState(const FCombatStateInitializeParameter& Parameter)
{
	InitParam = Parameter;
}

void UPlayerCombatStatePartialParry::UpdateState(float deltaTime)
{
	ElapsedTimeFromStateEnter += deltaTime;
}

float UPlayerCombatStatePartialParry::GetElapsedTimeFromStateEnter()
{
	return ElapsedTimeFromStateEnter;
}

EAnimationStateEnum UPlayerCombatStatePartialParry::GetAnimationStateEnum()
{
	return EAnimationStateEnum::PartialParry;
}

EAttackDirection UPlayerCombatStatePartialParry::GetPreparedAttackDirection() const
{
	return InitParam.StateEnterAttackDirection;
}

bool UPlayerCombatStatePartialParry::IsStateExpired()
{
	return ElapsedTimeFromStateEnter >= 1.0f;
}

void UPlayerCombatStatePartialParry::OnStateEnter()
{
	ElapsedTimeFromStateEnter = 0.0f;
	//ToDO: Montage 재생
	auto stateOwner = InitParam.OwnerCharacter;
	stateOwner.Get()->PlayMontage(InitParam.CombatMontageSet->GetPartialParryMontage());
}

void UPlayerCombatStatePartialParry::OnStateFinish()
{
}
