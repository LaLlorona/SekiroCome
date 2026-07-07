// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateMachineComponent.h"

#include "FCombatStateParameter.h"
#include "PlayerCombatState.h"
#include "PlayerCombatStateAttack.h"
#include "PlayerCombatStateGuard.h"
#include "PlayerCombatStateHit.h"
#include "PlayerCombatStateIdle.h"
#include "PlayerCombatStatePartialParry.h"
#include "PlayerCombatStatePerfectParryRiposte.h"


// Sets default values for this component's properties
UPlayerCombatStateMachineComponent::UPlayerCombatStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerCombatStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPlayerCombatStateMachineComponent::UpdateCombatState(float deltaTime)
{
	auto nextStateCandidate = PlayerCombatState->GetStateToTransition();
	if (nextStateCandidate.IsSet())
	{
		TryChangeState(nextStateCandidate.GetValue());
	}
	PlayerCombatState->UpdateState(deltaTime);
}

void UPlayerCombatStateMachineComponent::Initialize(const FCombatStateComponentInitializeParameter& Parameter)
{
	CombatStateInitializeParameter = Parameter;
	TryChangeStateByStateEnum(ECombatStateEnum::Idle);
}


#pragma warning(push)
#pragma warning(error: 4062)
void UPlayerCombatStateMachineComponent::TryChangeStateByStateEnum(ECombatStateEnum NewState)
{
	TScriptInterface<IPlayerCombatState> NewStateInstance;
	auto combatStateParam = FCombatStateParameter(CombatStateInitializeParameter, CurrentAttackDirection);
	switch (NewState)
	{
		case ECombatStateEnum::Idle:
			NewStateInstance = CreateCombatState<UPlayerCombatStateIdle>(combatStateParam);
			break;
		case ECombatStateEnum::Guard:
			NewStateInstance = CreateCombatState<UPlayerCombatStateGuard>(combatStateParam);
			break;
		case ECombatStateEnum::PartialParry:
			NewStateInstance = CreateCombatState<UPlayerCombatStatePartialParry>(combatStateParam);
			break;
		case ECombatStateEnum::PerfectParryRiposte:
			NewStateInstance = CreateCombatState<UPlayerCombatStatePerfectParryRiposte>(combatStateParam);
			break;
		case ECombatStateEnum::Hit:
			NewStateInstance = CreateCombatState<UPlayerCombatStateHit>(combatStateParam);
			break;
		case ECombatStateEnum::Attack:
			NewStateInstance = CreateCombatState<UPlayerCombatStateAttack>(combatStateParam);
			break;
	}
	TryChangeState(NewStateInstance);
}
#pragma warning(pop)

void UPlayerCombatStateMachineComponent::TryChangeState(TScriptInterface<IPlayerCombatState> NewState)
{
	if (PlayerCombatState)
	{
		PlayerCombatState->OnStateFinish();
	}
	PlayerCombatState = NewState;
	PlayerCombatState->OnStateEnter();
}

bool UPlayerCombatStateMachineComponent::CanParryNow() const
{
	auto state = Cast<UPlayerCombatStateGuard>(PlayerCombatState.GetObject());
	if (state != nullptr)
	{
		return state->CanParryNow();
	}
	return false;
}

void UPlayerCombatStateMachineComponent::OnAttackInputPressed()
{
	PlayerCombatState->OnAttackInputPressed();
}

EAnimationStateEnum UPlayerCombatStateMachineComponent::GetAnimationStateEnum()
{
	if (PlayerCombatState == nullptr)
	{
		return EAnimationStateEnum::Normal;
	}
	return PlayerCombatState->GetAnimationStateEnum();
}

EAttackDirection UPlayerCombatStateMachineComponent::GetPreparedAttackDirection() const
{
	return CurrentAttackDirection;
}

void UPlayerCombatStateMachineComponent::SetAttackDirection(EAttackDirection NewDirection)
{
	CurrentAttackDirection = NewDirection;
	OnAttackDirectionChanged.Broadcast(NewDirection);

	if (auto attackState = Cast<UPlayerCombatStateAttack>(PlayerCombatState.GetInterface()))
	{
		attackState->OnAttackDirectionManuallyChanged();
	}
}


