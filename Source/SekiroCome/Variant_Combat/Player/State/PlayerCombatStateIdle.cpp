// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateIdle.h"

#include "CombatTypes.h"

void UPlayerCombatStateIdle::UpdateState(float deltaTime)
{
}

float UPlayerCombatStateIdle::GetElapsedTimeFromStateEnter()
{
	return 0;
}

EAnimationStateEnum UPlayerCombatStateIdle::GetAnimationStateEnum()
{
	return EAnimationStateEnum::Normal;
}
