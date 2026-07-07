// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCombatStateParameter.h"
#include "PlayerCombatState.h"

#include "PlayerCombatStateIdle.generated.h"

/**
 *
 */
UCLASS()
class SEKIROCOME_API UPlayerCombatStateIdle : public UObject, public IPlayerCombatState
{
	GENERATED_BODY()

	UPROPERTY()
	FCombatStateParameter InitParam;

	UPROPERTY()
	TScriptInterface<IPlayerCombatState> PendingNextState;

public:
	virtual void InitializeState(const FCombatStateParameter& Parameter) override;
	virtual void UpdateState(float deltaTime) override;
	virtual float GetElapsedTimeFromStateEnter() override;
	virtual EAnimationStateEnum GetAnimationStateEnum() override;
	virtual void OnStateEnter() override;
	virtual void OnStateFinish() override;
	virtual void OnAttackInputPressed() override;
	virtual TOptional<TScriptInterface<IPlayerCombatState>> GetStateToTransition() override;
};
