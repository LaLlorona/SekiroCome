// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacterInputComponent.h"

#include "CombatCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedPlayerInput.h"
#include "CombatPlayerController.h"


// Sets default values for this component's properties
UCombatCharacterInputComponent::UCombatCharacterInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCombatCharacterInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCombatCharacterInputComponent::Move(const FInputActionValue& Value)
{
	if (!OwnerCharacter) return;
	FVector2D MovementVector = Value.Get<FVector2D>();
	OwnerCharacter->DoMove(MovementVector.X, MovementVector.Y);
}

void UCombatCharacterInputComponent::Look(const FInputActionValue& Value)
{
	if (!OwnerCharacter) return;
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	OwnerCharacter->DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void UCombatCharacterInputComponent::ComboAttackPressed()
{
	if (!OwnerCharacter) return;
	OwnerCharacter->DoComboAttackStart();
}

void UCombatCharacterInputComponent::ToggleCamera()
{
	if (!OwnerCharacter) return;
	OwnerCharacter->ToggleCamera();
}

void UCombatCharacterInputComponent::TryLockOnCamera()
{
	if (!OwnerCharacter) return;
	OwnerCharacter->TryLockOnCamera();
}

void UCombatCharacterInputComponent::TryGuardStart()
{
	if (!OwnerCharacter) return;
	OwnerCharacter->TryGuardStart();
}

void UCombatCharacterInputComponent::TryGuardEnd()
{
	if (!OwnerCharacter) return;
	OwnerCharacter->TryGuardEnd();
}

FVector2D UCombatCharacterInputComponent::GetCurrentMovementInput() const
{
	if (!OwnerCharacter) return FVector2D::ZeroVector;

	ACombatPlayerController* PC = Cast<ACombatPlayerController>(OwnerCharacter->GetController());
	if (!PC) return FVector2D::ZeroVector;

	const UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!Subsystem || !Subsystem->GetPlayerInput()) return FVector2D::ZeroVector;

	return Subsystem->GetPlayerInput()->GetActionValue(MoveAction).Get<FVector2D>();
}



// Called every frame
void UCombatCharacterInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatCharacterInputComponent::SetupBindings(UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UCombatCharacterInputComponent::Move);
		
	// Looking
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UCombatCharacterInputComponent::Look);
	EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &UCombatCharacterInputComponent::Look);

	// Combo Attack
	EnhancedInputComponent->BindAction(ComboAttackAction, ETriggerEvent::Started, this, &UCombatCharacterInputComponent::ComboAttackPressed);

	// Camera Side Toggle
	EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Triggered, this, &UCombatCharacterInputComponent::ToggleCamera);

	EnhancedInputComponent->BindAction(TryLockOnAction, ETriggerEvent::Started, this, &UCombatCharacterInputComponent::TryLockOnCamera);

	//guard
	EnhancedInputComponent->BindAction(TryGuardAction, ETriggerEvent::Started, this, &UCombatCharacterInputComponent::TryGuardStart);
	EnhancedInputComponent->BindAction(TryGuardAction, ETriggerEvent::Completed, this, &UCombatCharacterInputComponent::TryGuardEnd);
}

