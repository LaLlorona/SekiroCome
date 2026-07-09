// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateAttack.h"

#include "AnimNotify_AttackDirectionChange.h"
#include "AnimNotify_AttackTransitionWindowOpened.h"
#include "CombatCharacter.h"
#include "CombatLogic.h"
#include "FCombatStateParameter.h"
#include "PlayerCombatStateIdle.h"
#include "PlayerCombatStateMachineComponent.h"
#include "Table/CombatDataSubsystem.h"
#include "Table/CombatDataTableManager.h"
#include "Table/Row/FCombatAttackInfoRow.h"
#include "Table/Row/FCombatMontageRow.h"
#include "Table/Table/CombatAttackInfoTable.h"
#include "Table/Table/CombatMontageTable.h"
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
		auto direction = InitParam.StateComponentInitializeParameter.OwnerCharacter->GetCombatStateComponent()->GetPreparedAttackDirection();
		auto attackAnimRowName = CombatLogic::GetAttackInfoRowNameByDirection(direction);
		auto attackStateParam = FCombatStateParameter::CreateForAttack(InitParam.StateComponentInitializeParameter, attackAnimRowName);
		PendingNextState = CreateCombatState<UPlayerCombatStateAttack>(attackStateParam);
	}
}

void UPlayerCombatStateAttack::TryFallbackToIdleState()
{
	if (PendingNextState.GetObject() == nullptr && ElapsedTimeFromStateEnter >= CachedAttackStateDurationTime)
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
	UCombatDataTableManager* TableManager = UCombatDataSubsystem::GetCombatDataSubsystem(stateOwner)->GetInGameTableManager();

	FAttackInfoId AttackInfoRowName = InitParam.AttackInfoRowName.Get(FAttackInfoId(FName("Down")));
	const FCombatAttackInfoRow& AttackInfoRow = TableManager->CombatAttackInfoTable->FindByAttackInfoId(AttackInfoRowName);
	const FCombatMontageRow& MontageRow = TableManager->CombatMontageTable->FindByMontageId(AttackInfoRow.MontageNameKey);

	UAnimMontage* attackAnimMontage = MontageRow.Montage;
	float animMontagePlaySpeed = AttackInfoRow.MontagePlaySpeed;

	int32 sectionIndex = attackAnimMontage->GetSectionIndex(AttackInfoRow.MontageSectionName);
	CachedAttackStateDurationTime = attackAnimMontage->GetSectionLength(sectionIndex) / animMontagePlaySpeed;
	float sectionStartTime, sectionEndTime;
	attackAnimMontage->GetSectionStartAndEndTime(sectionIndex, sectionStartTime, sectionEndTime);
	for (auto NotifyEvent: attackAnimMontage->Notifies)
	{
		if (NotifyEvent.GetTriggerTime() >= sectionStartTime && NotifyEvent.GetTriggerTime() <= sectionEndTime)
		{
			if (Cast<UAnimNotify_AttackTransitionWindowOpened>(NotifyEvent.Notify))
			{
				CachedGetComboTransitionWindowTime = (NotifyEvent.GetTriggerTime() - sectionStartTime) / animMontagePlaySpeed;
				UtilityLogic::PrintString(FString::Printf(TEXT("Transition Time을 찾았습니다: %f"), CachedGetComboTransitionWindowTime));
			}
			else if (Cast<UAnimNotify_AttackDirectionChange>(NotifyEvent.Notify))
			{
				CachedAttackDirectionChangeTime = (NotifyEvent.GetTriggerTime() - sectionStartTime) / animMontagePlaySpeed;
			}
		}
	}
	stateOwner.Get()->PlayMontageWithSectionName(attackAnimMontage, AttackInfoRow.MontageSectionName, animMontagePlaySpeed);
}

void UPlayerCombatStateAttack::OnStateFinish()
{
}

void UPlayerCombatStateAttack::OnAttackInputPressed()
{
	bAttackInputPressed = true;
}


