// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ControllerPresentationComponent.generated.h"


enum class EAttackDirection : uint8;
class ACombatCharacter;
class UCombatAttackDirectionUI;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SEKIROCOME_API UControllerPresentationComponent : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION()
	void HandleAttackDirectionChanged(EAttackDirection AttackDirection);

	UFUNCTION()
	void HandleLockOnTargetChanged(bool bIsTargetExist);

public:
	// Sets default values for this component's properties
	UControllerPresentationComponent();
	UPROPERTY(EditAnywhere, Category="HUD")
	TSubclassOf<UCombatAttackDirectionUI> FourDirectionWidgetClass;
	UPROPERTY()
	TObjectPtr<UCombatAttackDirectionUI> FourDirectionWidget;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	
	void CreateAndShowWidget();

	void BindToCharacter(ACombatCharacter* Character);
};
