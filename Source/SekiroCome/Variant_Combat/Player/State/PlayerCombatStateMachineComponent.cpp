// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateMachineComponent.h"

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

void UPlayerCombatStateMachineComponent::Initialize(const FCombatStateInitializeParameter& Parameter)
{
	CombatStateInitializeParameter = Parameter;
	ChangeState(ECombatStateEnum::Idle);
}

void UPlayerCombatStateMachineComponent::TryChangeState(ECombatStateEnum NewState)
{
	//check whether current transition is possible
	ChangeState(NewState);
}

void UPlayerCombatStateMachineComponent::TryChangeState(TScriptInterface<IPlayerCombatState> NewState)
{
	//check whether current transition is possible
	ChangeState(NewState);
}

#pragma warning(push)
#pragma warning(error: 4062)
void UPlayerCombatStateMachineComponent::ChangeState(ECombatStateEnum NewState)
{
	TScriptInterface<IPlayerCombatState> NewStateInstance;
	switch (NewState)
	{
		case ECombatStateEnum::Idle:
			NewStateInstance = NewObject<UPlayerCombatStateIdle>();
			break;
		case ECombatStateEnum::Guard:
			NewStateInstance = NewObject<UPlayerCombatStateGuard>();
			break;
		case ECombatStateEnum::PartialParry:
			NewStateInstance = NewObject<UPlayerCombatStatePartialParry>();
			break;
		case ECombatStateEnum::PerfectParryRiposte:
			NewStateInstance = NewObject<UPlayerCombatStatePerfectParryRiposte>();
			break;
		case ECombatStateEnum::Hit:
			NewStateInstance = NewObject<UPlayerCombatStateHit>();
			break;
		case ECombatStateEnum::Attack:
			NewStateInstance = NewObject<UPlayerCombatStateAttack>();
			break;
	}
	ChangeState(NewStateInstance);
}
#pragma warning(pop)

void UPlayerCombatStateMachineComponent::ChangeState(TScriptInterface<IPlayerCombatState> NewState)
{
	if (PlayerCombatState)
	{
		PlayerCombatState->OnStateFinish();
	}
	PlayerCombatState = NewState;
	PlayerCombatState->InitializeState(CombatStateInitializeParameter);
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
}


