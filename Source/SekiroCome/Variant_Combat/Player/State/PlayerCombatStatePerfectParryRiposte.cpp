// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStatePerfectParryRiposte.h"

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
