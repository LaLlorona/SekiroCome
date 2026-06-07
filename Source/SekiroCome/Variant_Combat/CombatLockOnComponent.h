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
	
	
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void TryLockOn(FVector CameraLocation, FVector CameraForward);

	FLockOnResult GetLockOnResult() const;
};
