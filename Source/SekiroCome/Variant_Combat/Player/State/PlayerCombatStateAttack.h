// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FCombatStateParameter.h"
#include "PlayerCombatState.h"

#include "PlayerCombatStateAttack.generated.h"

/**
 *
 */
UCLASS()
class SEKIROCOME_API UPlayerCombatStateAttack : public UObject, public IPlayerCombatState
{
	GENERATED_BODY()
	float ElapsedTimeFromStateEnter;

	UPROPERTY()
	FCombatStateParameter InitParam;

	// Next Attack state reserved when AttackInput is pressed. If a transition isn't allowed yet, GetStateToTransition() holds it until it is.
	UPROPERTY()
	TScriptInterface<IPlayerCombatState> PendingNextState;



public:
	virtual void InitializeState(const FCombatStateParameter& Parameter) override;
	void TryAutoChangeAttackDirection();
	void TryConsumeBufferedAttackInput();
	void TryFallbackToIdleState();
	virtual void UpdateState(float deltaTime) override;
	virtual float GetElapsedTimeFromStateEnter() override;
	virtual EAnimationStateEnum GetAnimationStateEnum() override;
	virtual void OnStateEnter() override;
	virtual void OnStateFinish() override;
	virtual void OnAttackInputPressed() override;
	virtual TOptional<TScriptInterface<IPlayerCombatState>> GetStateToTransition() override;

	void OnAttackDirectionManuallyChanged();
private:

	float CachedGetComboTransitionWindowTime;
	float CachedAttackDirectionChangeTime;
	float CachedAttackStateDurationTime;

	// Only inputs pressed after this time are reflected as bAttackInputPressed = true. If there is no Notify, it stays 0.0f and input is always accepted immediately.
	float CachedAttackInputWindowOpenTime;

	// Whether SetAttackDirection has been called at least once since entering AttackState.
	// Once true, automatic direction change via GetNextAttackDirectionOnAttack is no longer attempted.
	bool bAttackDirectionManuallyChanged = false;

	// Flag that only remembers that AttackInput was pressed. The actual PendingNextState is created
	// in UpdateState once CachedGetComboTransitionWindowTime has passed.
	bool bAttackInputPressed = false;
};
