// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.h"
#include "FCombatStateInitializeParameter.h"

#include "PlayerCombatStateMachineComponent.generated.h"
struct FCombatStateInitializeParameter;
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
	void Initialize(const FCombatStateInitializeParameter& Parameter);
	void TryChangeState(ECombatStateEnum NewState);
	

	bool CanParryNow() const ;
	bool IsBeingHit() const;

	EAnimationStateEnum GetAnimationStateEnum();
	EAttackDirection GetPreparedAttackDirection() const;
	ECombatStateEnum GetCurrentStateEnum() const { return CurrentStateEnum; }

private:
	void ChangeState(ECombatStateEnum NewState);

	UPROPERTY()
	FCombatStateInitializeParameter CombatStateInitializeParameter;

	ECombatStateEnum CurrentStateEnum = ECombatStateEnum::Idle;
};
