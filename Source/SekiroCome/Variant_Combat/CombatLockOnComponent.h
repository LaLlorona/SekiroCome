// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatLockOnComponent.generated.h"


struct FLockOnTargetNotExist {};

struct FLockOnTargetExist
{
	FVector TargetPosition;
};

using FLockOnResult = TVariant<FLockOnTargetNotExist, FLockOnTargetExist>;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SEKIROCOME_API UCombatLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLockOnTargetChanged, bool, bIsTargetExist);

public:
	// Sets default values for this component's properties
	UCombatLockOnComponent();

protected:
	// Called when the game starts


	UPROPERTY(EditAnyWhere)
	float LockOnTargetSearchDistance;

	UPROPERTY(EditAnyWhere)
	float LockOnTargetSearchAngle;

	
	TWeakObjectPtr<AActor> LockedOnTarget;

	// Whether a target existed at the previous CustomUpdate. Used for comparison so OnLockOnTargetChanged is broadcast only on frames where the value changed.
	bool bWasTargetExist = false;


	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void TryLockOn(FVector CameraLocation, FVector CameraForward);

	FLockOnResult GetLockOnResult() const;

	/** True if the locked-on target implements ICombatDamageable and its PerfectBlock/MasterStrike judgement window is currently open (§5-1). False if there is no target or it doesn't implement the interface. */
	bool IsLockOnTargetParryWindowOpen() const;

	// Called explicitly from the owning Character's Tick. Broadcasts OnLockOnTargetChanged only when target existence actually changed.
	void CustomUpdate();

	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnLockOnTargetChanged OnLockOnTargetChanged;
};
