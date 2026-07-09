// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateHit.h"

#include "CombatCharacter.h"
#include "FCombatStateParameter.h"
#include "PlayerCombatStateIdle.h"
#include "Table/CombatDataSubsystem.h"
#include "Table/CombatDataTableManager.h"
#include "Table/Table/CombatTuningDataTable.h"

void UPlayerCombatStateHit::InitializeState(const FCombatStateParameter& Parameter)
{
	InitParam = Parameter;
}

void UPlayerCombatStateHit::UpdateState(float deltaTime)
{
	ElapsedTimeFromStateEnter += deltaTime;
}

float UPlayerCombatStateHit::GetElapsedTimeFromStateEnter()
{
	return ElapsedTimeFromStateEnter;
}

EAnimationStateEnum UPlayerCombatStateHit::GetAnimationStateEnum()
{
	return EAnimationStateEnum::Hit;
}

TOptional<TScriptInterface<IPlayerCombatState>> UPlayerCombatStateHit::GetStateToTransition()
{
	ACombatCharacter* OwnerCharacter = InitParam.StateComponentInitializeParameter.OwnerCharacter.Get();
	const FCombatTuningRow& Row = UCombatDataSubsystem::GetCombatDataSubsystem(OwnerCharacter)->GetInGameTableManager()->CombatTuningDataTable->FindByCombatTuningId(OwnerCharacter->GetCombatTuningRowName());

	if (ElapsedTimeFromStateEnter >= Row.HitStunDurationInSecond)
	{
		auto attackStateParam = FCombatStateParameter::CreateWithPreparedAttackDirection(InitParam.StateComponentInitializeParameter);
		TScriptInterface<IPlayerCombatState> NextState = CreateCombatState<UPlayerCombatStateIdle>(attackStateParam);
		return NextState;
	}
	return {};
}

void UPlayerCombatStateHit::OnStateEnter()
{
	ElapsedTimeFromStateEnter = 0.0f;
}

void UPlayerCombatStateHit::OnStateFinish()
{
}

void UPlayerCombatStateHit::OnAttackInputPressed()
{
}
