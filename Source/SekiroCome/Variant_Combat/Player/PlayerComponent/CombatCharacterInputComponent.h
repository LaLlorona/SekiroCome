// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.h"

#include "CombatCharacterInputComponent.generated.h"


class UInputAction;
class UEnhancedInputComponent;
class ACombatCharacter;
class UMouseInputThresholdCheckComponent;
struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SEKIROCOME_API UCombatCharacterInputComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<ACombatCharacter> OwnerCharacter;

	UPROPERTY(VisibleAnywhere, Instanced, Category="Components")
	TObjectPtr<UMouseInputThresholdCheckComponent> MouseThresholdComponent;

public:
	// Sets default values for this component's properties
	UCombatCharacterInputComponent();
	void Initialize(ACombatCharacter* InOwnerCharacter) { OwnerCharacter = InOwnerCharacter; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ComboAttackPressed();
	void ToggleCamera();
	void TryLockOnCamera();
	void TryGuardStart();
	void TryGuardEnd();

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	/** Combo Attack Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* ComboAttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TryGuardAction;

	/** Toggle Camera Side Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* ToggleCameraAction;

	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* TryLockOnAction;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetupBindings(UEnhancedInputComponent* EnhancedInputComponent);
	bool GetMoveAttackDirection(EAttackDirection& OutDirection) const;
	bool GetMouseAttackDirection(float DeltaTime, EAttackDirection& OutDirection) const;
};
