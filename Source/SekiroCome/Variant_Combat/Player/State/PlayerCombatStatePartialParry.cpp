// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStatePartialParry.h"

#include "CombatCharacter.h"
#include "FCombatStateParameter.h"
#include "Montage/CombatMontageSet.h"
#include "PlayerCombatStateIdle.h"

void UPlayerCombatStatePartialParry::InitializeState(const FCombatStateParameter& Parameter)
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

TOptional<TScriptInterface<IPlayerCombatState>> UPlayerCombatStatePartialParry::GetStateToTransition()
{
	if (ElapsedTimeFromStateEnter >= 1.0f)
	{
		auto attackStateParam = FCombatStateParameter::CreateWithPreparedAttackDirection(InitParam.StateComponentInitializeParameter);
		TScriptInterface<IPlayerCombatState> NextState = CreateCombatState<UPlayerCombatStateIdle>(attackStateParam);
		return NextState;
	}
	return {};
}

void UPlayerCombatStatePartialParry::OnStateEnter()
{
	ElapsedTimeFromStateEnter = 0.0f;
	//ToDO: Play Montage
	auto stateOwner = InitParam.StateComponentInitializeParameter.OwnerCharacter;
	/*stateOwner.Get()->PlayMontage(InitParam.StateComponentInitializeParameter.CombatMontageSet->GetPartialParryMontage());*/
}

void UPlayerCombatStatePartialParry::OnStateFinish()
{
}

void UPlayerCombatStatePartialParry::OnAttackInputPressed()
{
}
