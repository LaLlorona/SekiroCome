// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateAttack.h"

#include "AnimNotify_AttackDirectionChange.h"
#include "AnimNotify_AttackTransitionWindowOpened.h"
#include "CombatCharacter.h"
#include "CombatLogic.h"
#include "FCombatStateParameter.h"
#include "PlayerCombatStateIdle.h"
#include "PlayerCombatStateMachineComponent.h"
#include "Montage/CombatMontageSet.h"
#include "Utility/UtilityLogic.h"

void UPlayerCombatStateAttack::InitializeState(const FCombatStateParameter& Parameter)
{
	InitParam = Parameter;
}

void UPlayerCombatStateAttack::TryAutoChangeAttackDirection()
{
	if (!bAttackDirectionManuallyChanged && ElapsedTimeFromStateEnter >= CachedAttackDirectionChangeTime)
	{
		auto stateOwner = InitParam.StateComponentInitializeParameter.OwnerCharacter;
		auto nextDirection = CombatLogic::GetNextAttackDirectionOnAttack(InitParam.StateEnterAttackDirection);
		stateOwner->GetCombatStateComponent()->SetAttackDirection(nextDirection);
	}
}

void UPlayerCombatStateAttack::TryConsumeBufferedAttackInput()
{
	if (bAttackInputPressed && PendingNextState.GetObject() == nullptr && ElapsedTimeFromStateEnter >= CachedGetComboTransitionWindowTime)
	{
		auto attackStateParam = FCombatStateParameter::CreateWithPreparedAttackDirection(InitParam.StateComponentInitializeParameter);
		PendingNextState = CreateCombatState<UPlayerCombatStateAttack>(attackStateParam);
	}
}

void UPlayerCombatStateAttack::TryFallbackToIdleState()
{
	if (PendingNextState.GetObject() == nullptr && ElapsedTimeFromStateEnter >= 1.0f)
	{
		auto attackStateParam = FCombatStateParameter::CreateWithPreparedAttackDirection(InitParam.StateComponentInitializeParameter);
		TScriptInterface<IPlayerCombatState> NextState = CreateCombatState<UPlayerCombatStateIdle>(attackStateParam);
		PendingNextState = NextState;
	}
}

void UPlayerCombatStateAttack::UpdateState(float deltaTime)
{
	ElapsedTimeFromStateEnter += deltaTime;

	TryAutoChangeAttackDirection();

	TryConsumeBufferedAttackInput();

	TryFallbackToIdleState();
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

	if (PendingNextState.GetObject() != nullptr)
	{
		return PendingNextState;
	}
	return {};
}

void UPlayerCombatStateAttack::OnAttackDirectionManuallyChanged()
{
	bAttackDirectionManuallyChanged = true;
}

void UPlayerCombatStateAttack::OnStateEnter()
{
	ElapsedTimeFromStateEnter = 0.0f;
	bAttackDirectionManuallyChanged = false;
	CachedAttackDirectionChangeTime = TNumericLimits<float>::Max();

	auto stateOwner = InitParam.StateComponentInitializeParameter.OwnerCharacter;
	auto attackAnimMontage = InitParam.StateComponentInitializeParameter.CombatMontageSet->GetAttackAnimMontage();
	auto attackSectionName = CombatLogic::GetAnimationSectionNameByAttackDirection(InitParam.StateEnterAttackDirection);


	int32 sectionIndex = attackAnimMontage->GetSectionIndex(attackSectionName);
	float sectionStartTime, sectionEndTime;
	attackAnimMontage->GetSectionStartAndEndTime(sectionIndex, sectionStartTime, sectionEndTime);
	for (auto NotifyEvent: attackAnimMontage->Notifies)
	{
		if (NotifyEvent.GetTriggerTime() >= sectionStartTime && NotifyEvent.GetTriggerTime() <= sectionEndTime)
		{
			if (Cast<UAnimNotify_AttackTransitionWindowOpened>(NotifyEvent.Notify))
			{
				CachedGetComboTransitionWindowTime = NotifyEvent.GetTriggerTime() - sectionStartTime;
				UtilityLogic::PrintString(FString::Printf(TEXT("Transition Time을 찾았습니다: %f"), CachedGetComboTransitionWindowTime));
			}
			else if (Cast<UAnimNotify_AttackDirectionChange>(NotifyEvent.Notify))
			{
				CachedAttackDirectionChangeTime = NotifyEvent.GetTriggerTime() - sectionStartTime;
			}
		}
	}
	stateOwner.Get()->PlayMontageWithSectionName(attackAnimMontage, attackSectionName);
	
}

void UPlayerCombatStateAttack::OnStateFinish()
{
}

void UPlayerCombatStateAttack::OnAttackInputPressed()
{
	bAttackInputPressed = true;
}


