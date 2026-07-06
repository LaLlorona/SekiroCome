// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateAttack.h"

#include "CombatCharacter.h"
#include "FCombatStateInitializeParameter.h"
#include "PlayerCombatStateIdle.h"
#include "Montage/CombatMontageSet.h"

void UPlayerCombatStateAttack::InitializeState(const FCombatStateInitializeParameter& Parameter)
{
	InitParam = Parameter;
}

void UPlayerCombatStateAttack::UpdateState(float deltaTime)
{
	ElapsedTimeFromStateEnter += deltaTime;
	if (ElapsedTimeFromStateEnter >= 1.0f)
	{
		TScriptInterface<IPlayerCombatState> NextState = NewObject<UPlayerCombatStateIdle>();
		PendingNextState = NextState;
	}
}

float UPlayerCombatStateAttack::GetElapsedTimeFromStateEnter()
{
	return ElapsedTimeFromStateEnter;
}

EAnimationStateEnum UPlayerCombatStateAttack::GetAnimationStateEnum()
{
	return EAnimationStateEnum::Normal;
}

TOptional<TScriptInterface<IPlayerCombatState>> UPlayerCombatStateAttack::GetStateToTransition()
{
	// TODO: 몽타주의 AnimNotify로 대체 예정. 지금은 시작 후 0.3초가 지나면 전환 가능하다고 가정한다.
	constexpr float ComboTransitionWindowTime = 0.3f;

	if (PendingNextState.GetObject() != nullptr && ElapsedTimeFromStateEnter >= ComboTransitionWindowTime)
	{
		return PendingNextState;
	}
	return {};
}

void UPlayerCombatStateAttack::OnStateEnter()
{
	ElapsedTimeFromStateEnter = 0.0f;
	auto stateOwner = InitParam.OwnerCharacter;
	stateOwner.Get()->PlayMontage(InitParam.CombatMontageSet->GetAttackAnimMontage());

}

void UPlayerCombatStateAttack::OnStateFinish()
{
}

void UPlayerCombatStateAttack::OnAttackInputPressed()
{
	PendingNextState = NewObject<UPlayerCombatStateAttack>();
}


