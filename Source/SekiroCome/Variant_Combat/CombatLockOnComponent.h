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

	// 직전 CustomUpdate 시점의 타겟 존재 여부. 값이 바뀐 프레임에만 OnLockOnTargetChanged를 Broadcast하기 위한 비교용.
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

	// 소유 Character의 Tick에서 명시적으로 호출. 타겟 존재 여부가 실제로 바뀐 경우에만 OnLockOnTargetChanged를 Broadcast한다.
	void CustomUpdate();

	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnLockOnTargetChanged OnLockOnTargetChanged;
};
