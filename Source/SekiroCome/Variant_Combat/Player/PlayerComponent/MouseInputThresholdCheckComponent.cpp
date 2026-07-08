// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseInputThresholdCheckComponent.h"

UMouseInputThresholdCheckComponent::UMouseInputThresholdCheckComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UMouseInputThresholdCheckComponent::CustomUpdate(float DeltaTime, const FVector2D& MouseDelta, EAttackDirection& OutDirection)
{
	AccumulatedMouseDelta += MouseDelta;
	AccumulatedMouseDelta *= FMath::Exp(-DecayRate * DeltaTime);

	if (AccumulatedMouseDelta.Size() < TriggerThreshold)
	{
		return false;
	}

	if (FMath::Abs(AccumulatedMouseDelta.X) > FMath::Abs(AccumulatedMouseDelta.Y))
	{
		OutDirection = AccumulatedMouseDelta.X > 0 ? EAttackDirection::Right : EAttackDirection::Left;
	}
	else
	{
		OutDirection = AccumulatedMouseDelta.Y > 0 ? EAttackDirection::Down : EAttackDirection::Up;
	}

	AccumulatedMouseDelta = FVector2D::ZeroVector;
	return true;
}
