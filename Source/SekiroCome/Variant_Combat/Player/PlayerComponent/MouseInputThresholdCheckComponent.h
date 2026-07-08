// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.h"

#include "MouseInputThresholdCheckComponent.generated.h"

// 마우스의 프레임별 이동량을 누적하고 시간에 따라 감쇠(leaky accumulator)시켜,
// 클릭-드래그 없이 상시 추적만으로 상/하/좌/우 "플릭"을 감지하는 컴포넌트.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SEKIROCOME_API UMouseInputThresholdCheckComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMouseInputThresholdCheckComponent();

	// 매 프레임 호출. 이번 프레임의 MouseDelta를 누적/감쇠시키고,
	// TriggerThreshold를 넘으면 누적값을 리셋한 뒤 true와 함께 OutDirection을 채운다.
	bool CustomUpdate(float DeltaTime, const FVector2D& MouseDelta, EAttackDirection& OutDirection);

protected:
	// 누적값이 시간에 따라 줄어드는 속도. 클수록 마우스를 멈췄을 때 더 빨리 리셋된다.
	UPROPERTY(EditAnywhere, Category="MouseThreshold")
	float DecayRate = 8.0f;

	// 누적된 이동량이 이 값을 넘으면 플릭으로 판정한다.
	UPROPERTY(EditAnywhere, Category="MouseThreshold")
	float TriggerThreshold = 40.0f;

private:
	FVector2D AccumulatedMouseDelta = FVector2D::ZeroVector;
};
