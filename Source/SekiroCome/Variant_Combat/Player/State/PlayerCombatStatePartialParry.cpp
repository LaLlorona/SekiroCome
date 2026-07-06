// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStatePartialParry.h"

#include "CombatCharacter.h"
#include "FCombatStateInitializeParameter.h"
#include "Montage/CombatMontageSet.h"
#include "PlayerCombatStateIdle.h"

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

TOptional<TScriptInterface<IPlayerCombatState>> UPlayerCombatStatePartialParry::GetStateToTransition()
{
	if (ElapsedTimeFromStateEnter >= 1.0f)
	{
		TScriptInterface<IPlayerCombatState> NextState = NewObject<UPlayerCombatStateIdle>();
		return NextState;
	}
	return {};
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

void UPlayerCombatStatePartialParry::OnAttackInputPressed()
{
}
