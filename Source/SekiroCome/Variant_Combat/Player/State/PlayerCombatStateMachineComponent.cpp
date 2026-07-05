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
	if (PlayerCombatState->IsStateExpired())
	{
		TryChangeState(ECombatStateEnum::Idle);
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

#pragma warning(push)
#pragma warning(error: 4062) 
void UPlayerCombatStateMachineComponent::ChangeState(ECombatStateEnum NewState)
{
	if (PlayerCombatState)
	{
		// hand off the direction the outgoing state ended with to the incoming state's init parameter
		CombatStateInitializeParameter.StateEnterAttackDirection = PlayerCombatState->GetPreparedAttackDirection();
		PlayerCombatState->OnStateFinish();
	}
	switch (NewState)
	{
		case ECombatStateEnum::Idle:
			PlayerCombatState = NewObject<UPlayerCombatStateIdle>();
			break;
		case ECombatStateEnum::Guard:
			PlayerCombatState = NewObject<UPlayerCombatStateGuard>();
			break;
		case ECombatStateEnum::PartialParry:
			PlayerCombatState = NewObject<UPlayerCombatStatePartialParry>();
			break;
		case ECombatStateEnum::PerfectParryRiposte:
			PlayerCombatState = NewObject<UPlayerCombatStatePerfectParryRiposte>();
			break;
		case ECombatStateEnum::Hit:
			PlayerCombatState = NewObject<UPlayerCombatStateHit>();
			break;
		case ECombatStateEnum::Attack:
			PlayerCombatState = NewObject<UPlayerCombatStateAttack>();
			break;
	}
	CurrentStateEnum = NewState;
	PlayerCombatState->InitializeState(CombatStateInitializeParameter);
	PlayerCombatState->OnStateEnter();
}
#pragma warning(pop)

bool UPlayerCombatStateMachineComponent::CanParryNow() const
{
	auto state = Cast<UPlayerCombatStateGuard>(PlayerCombatState.GetObject());
	if (state != nullptr)
	{
		return state->CanParryNow();
	}
	return false;
}

bool UPlayerCombatStateMachineComponent::IsBeingHit() const
{
	return Cast<UPlayerCombatStateHit>(PlayerCombatState.GetObject()) != nullptr;
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
	if (PlayerCombatState == nullptr)
	{
		return EAttackDirection::Down;
	}
	return PlayerCombatState->GetPreparedAttackDirection();
}


