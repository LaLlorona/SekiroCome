// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatStateMachineComponent.h"

#include "PlayerCombatState.h"
#include "PlayerCombatStateGuard.h"
#include "PlayerCombatStateIdle.h"


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


// Called every frame
void UPlayerCombatStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerCombatStateMachineComponent::Initialize()
{
	PlayerCombatState = NewObject<UPlayerCombatStateIdle>();
}

void UPlayerCombatStateMachineComponent::TryChangeToBlockState()
{
	PlayerCombatState = NewObject<UPlayerCombatStateGuard>();
}

void UPlayerCombatStateMachineComponent::TryChangeToIdleState()
{
	PlayerCombatState = NewObject<UPlayerCombatStateIdle>();
}

EAnimationStateEnum UPlayerCombatStateMachineComponent::GetAnimationStateEnum()
{
	return PlayerCombatState->GetAnimationStateEnum();
}


