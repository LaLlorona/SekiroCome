// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStatePerfectParryRiposte.h"

#include "FCombatStateInitializeParameter.h"

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

bool UPlayerCombatStatePerfectParryRiposte::IsStateExpired()
{
	return ElapsedTimeFromStateEnter >= 1.0f;
}

void UPlayerCombatStatePerfectParryRiposte::OnStateEnter()
{
	ElapsedTimeFromStateEnter = 0.0f;
}

void UPlayerCombatStatePerfectParryRiposte::OnStateFinish()
{
}
