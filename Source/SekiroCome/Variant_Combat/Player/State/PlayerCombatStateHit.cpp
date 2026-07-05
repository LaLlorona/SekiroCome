// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateHit.h"

#include "CombatCharacter.h"
#include "FCombatStateInitializeParameter.h"
#include "Table/CombatDataSubsystem.h"
#include "Table/CombatDataTableManager.h"
#include "Table/CombatTuningDataTable.h"

void UPlayerCombatStateHit::InitializeState(const FCombatStateInitializeParameter& Parameter)
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

EAttackDirection UPlayerCombatStateHit::GetPreparedAttackDirection() const
{
	return InitParam.StateEnterAttackDirection;
}

bool UPlayerCombatStateHit::IsStateExpired()
{
	ACombatCharacter* OwnerCharacter = InitParam.OwnerCharacter.Get();
	const FCombatTuningRow& Row = UCombatDataSubsystem::GetCombatDataSubsystem(OwnerCharacter)->GetInGameTableManager()->CombatTuningDataTable->FindByRowNameOrThrow(OwnerCharacter->GetCombatTuningRowName());

	return ElapsedTimeFromStateEnter >= Row.HitStunDurationInSecond;
}

void UPlayerCombatStateHit::OnStateEnter()
{
	ElapsedTimeFromStateEnter = 0.0f;
}

void UPlayerCombatStateHit::OnStateFinish()
{
}
