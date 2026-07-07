// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.h"
#include "FCombatStateComponentInitializeParameter.h"

#include "PlayerCombatStateMachineComponent.generated.h"
struct FCombatStateComponentInitializeParameter;
class IPlayerCombatState;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))

class SEKIROCOME_API UPlayerCombatStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TScriptInterface<IPlayerCombatState> PlayerCombatState;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackDirectionChanged, EAttackDirection, NewDirection);


	

public:
	// Sets default values for this component's properties
	UPlayerCombatStateMachineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	void UpdateCombatState(float deltaTime);
	void Initialize(const FCombatStateComponentInitializeParameter& Parameter);
	void TryChangeStateByStateEnum(ECombatStateEnum NewState);
	void TryChangeState(TScriptInterface<IPlayerCombatState> NewState);


	bool CanParryNow() const ;
	void OnAttackInputPressed();
	
	EAnimationStateEnum GetAnimationStateEnum();
	EAttackDirection GetPreparedAttackDirection() const;
	void SetAttackDirection(EAttackDirection NewDirection);

	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnAttackDirectionChanged OnAttackDirectionChanged;

private:

	UPROPERTY()
	FCombatStateComponentInitializeParameter CombatStateInitializeParameter;
	
	EAttackDirection CurrentAttackDirection = EAttackDirection::Down;
};
