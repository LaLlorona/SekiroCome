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

	// AttackInput이 눌렸을 때 예약해두는 다음 Attack 상태. 아직 전환 가능 시점이 아니면 GetStateToTransition()에서 대기시킨다.
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

	// 이 시점 이후에 눌린 입력만 bAttackInputPressed = true로 반영된다. Notify가 없으면 0.0f로 남아 항상 즉시 허용된다.
	float CachedAttackInputWindowOpenTime;

	// AttackState 진입 이후 SetAttackDirection이 한 번이라도 호출됐는지 여부.
	// true가 되면 GetNextAttackDirectionOnAttack에 의한 자동 방향 전환을 더 이상 시도하지 않는다.
	bool bAttackDirectionManuallyChanged = false;

	// AttackInput이 눌렸다는 사실만 기억해두는 플래그. 실제 PendingNextState 생성은
	// CachedGetComboTransitionWindowTime이 지난 시점에 UpdateState에서 수행한다.
	bool bAttackInputPressed = false;
};
