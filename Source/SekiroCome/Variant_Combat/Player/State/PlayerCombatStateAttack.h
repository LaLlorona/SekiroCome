// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FCombatStateInitializeParameter.h"
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
	FCombatStateInitializeParameter InitParam;

	// AttackInput이 눌렸을 때 예약해두는 다음 Attack 상태. 아직 전환 가능 시점이 아니면 GetStateToTransition()에서 대기시킨다.
	UPROPERTY()
	TScriptInterface<IPlayerCombatState> PendingNextState;

public:
	virtual void InitializeState(const FCombatStateInitializeParameter& Parameter) override;
	virtual void UpdateState(float deltaTime) override;
	virtual float GetElapsedTimeFromStateEnter() override;
	virtual EAnimationStateEnum GetAnimationStateEnum() override;
	virtual void OnStateEnter() override;
	virtual void OnStateFinish() override;
	virtual void OnAttackInputPressed() override;
	virtual TOptional<TScriptInterface<IPlayerCombatState>> GetStateToTransition() override;
};
