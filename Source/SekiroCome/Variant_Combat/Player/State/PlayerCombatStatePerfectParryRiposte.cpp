// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStatePerfectParryRiposte.h"

#include "CombatCharacter.h"
#include "FCombatStateParameter.h"
#include "Montage/CombatMontageSet.h"
#include "PlayerCombatStateIdle.h"

void UPlayerCombatStatePerfectParryRiposte::InitializeState(const FCombatStateParameter& Parameter)
{
	InitParam = Parameter;
}

void UPlayerCombatStatePerfectParryRiposte::UpdateState(float deltaTime)
{
	ElapsedTimeFromStateEnter += deltaTime;
}

float UPlayerCombatStatePerfectParryRiposte::GetElapsedTimeFromStateEnter()
{
	return ElapsedTimeFromStateEnter;
}

EAnimationStateEnum UPlayerCombatStatePerfectParryRiposte::GetAnimationStateEnum()
{
	return EAnimationStateEnum::Riposte;
}

TOptional<TScriptInterface<IPlayerCombatState>> UPlayerCombatStatePerfectParryRiposte::GetStateToTransition()
{
	if (ElapsedTimeFromStateEnter >= 1.0f)
	{
		auto attackStateParam = FCombatStateParameter::CreateWithPreparedAttackDirection(InitParam.StateComponentInitializeParameter);
		TScriptInterface<IPlayerCombatState> NextState = CreateCombatState<UPlayerCombatStateIdle>(attackStateParam);
		return NextState;
	}
	return {};
}

void UPlayerCombatStatePerfectParryRiposte::OnStateEnter()
{
	ElapsedTimeFromStateEnter = 0.0f;
	//ToDO: Play Montage
	auto stateOwner = InitParam.StateComponentInitializeParameter.OwnerCharacter;
	/*stateOwner.Get()->PlayMontage(InitParam.StateComponentInitializeParameter.CombatMontageSet->GetRiposteMontage());*/
	
}

void UPlayerCombatStatePerfectParryRiposte::OnStateFinish()
{
}

void UPlayerCombatStatePerfectParryRiposte::OnAttackInputPressed()
{
}
