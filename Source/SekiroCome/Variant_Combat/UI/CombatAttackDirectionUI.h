// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatAttackDirectionUI.generated.h"

enum class EAttackDirection : uint8;
/**
 *
 */
UCLASS()
class SEKIROCOME_API UCombatAttackDirectionUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="Set Direction UI")
	void SetDirectionalUI(EAttackDirection DirectionEnum);

	
};
