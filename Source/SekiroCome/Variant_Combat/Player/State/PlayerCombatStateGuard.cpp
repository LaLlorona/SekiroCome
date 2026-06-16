// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateGuard.h"

#include "CombatTypes.h"

void UPlayerCombatStateGuard::UpdateState(float deltaTime)
{
}

float UPlayerCombatStateGuard::GetElapsedTimeFromStateEnter()
{
	return 0;
}

EAnimationStateEnum UPlayerCombatStateGuard::GetAnimationStateEnum()
{
	return EAnimationStateEnum::Guard;
}
