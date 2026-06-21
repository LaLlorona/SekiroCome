// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.h"

#include "PlayerCombatStateMachineComponent.generated.h"
class IPlayerCombatState;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SEKIROCOME_API UPlayerCombatStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TScriptInterface<IPlayerCombatState> PlayerCombatState;

public:
	// Sets default values for this component's properties
	UPlayerCombatStateMachineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	void UpdateCombatState(float deltaTime);
	void Initialize();
	void TryChangeToBlockState();
	void TryChangeToIdleState();
	void TryChangeToRiposteState();
	bool CanParryNow() const ;

	EAnimationStateEnum GetAnimationStateEnum();
};
