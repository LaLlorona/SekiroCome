// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.h"

#include "MouseInputThresholdCheckComponent.generated.h"

// Component that accumulates per-frame mouse movement and decays it over time (leaky accumulator),
// detecting up/down/left/right "flicks" through continuous tracking alone, without click-and-drag.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SEKIROCOME_API UMouseInputThresholdCheckComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMouseInputThresholdCheckComponent();

	// Called every frame. Accumulates/decays this frame's MouseDelta, and when it exceeds
	// TriggerThreshold, resets the accumulated value, fills OutDirection, and returns true.
	bool CustomUpdate(float DeltaTime, const FVector2D& MouseDelta, EAttackDirection& OutDirection);

protected:
	// Rate at which the accumulated value decays over time. Higher values reset faster once the mouse stops.
	UPROPERTY(EditAnywhere, Category="MouseThreshold")
	float DecayRate = 8.0f;

	// A flick is registered when the accumulated movement exceeds this value.
	UPROPERTY(EditAnywhere, Category="MouseThreshold")
	float TriggerThreshold = 40.0f;

private:
	FVector2D AccumulatedMouseDelta = FVector2D::ZeroVector;
};
