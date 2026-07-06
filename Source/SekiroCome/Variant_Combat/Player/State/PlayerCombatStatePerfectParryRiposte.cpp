// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStatePerfectParryRiposte.h"

#include "CombatCharacter.h"
#include "FCombatStateInitializeParameter.h"
#include "Montage/CombatMontageSet.h"
#include "PlayerCombatStateIdle.h"

void UPlayerCombatStatePerfectParryRiposte::InitializeState(const FCombatStateInitializeParameter& Parameter)
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
		TScriptInterface<IPlayerCombatState> NextState = NewObject<UPlayerCombatStateIdle>();
		return NextState;
	}
	return {};
}

void UPlayerCombatStatePerfectParryRiposte::OnStateEnter()
{
	ElapsedTimeFromStateEnter = 0.0f;
	//ToDO: Montage 재생
	auto stateOwner = InitParam.OwnerCharacter;
	stateOwner.Get()->PlayMontage(InitParam.CombatMontageSet->GetRiposteMontage());
	
}

void UPlayerCombatStatePerfectParryRiposte::OnStateFinish()
{
}

void UPlayerCombatStatePerfectParryRiposte::OnAttackInputPressed()
{
}
