// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerPresentationComponent.h"

#include "CombatAttackDirectionUI.h"
#include "CombatCharacter.h"
#include "CombatLockOnComponent.h"
#include "Blueprint/UserWidget.h"
#include "Player/State/PlayerCombatStateMachineComponent.h"


void UControllerPresentationComponent::HandleAttackDirectionChanged(EAttackDirection AttackDirection)
{
	FourDirectionWidget->SetDirectionalUI(AttackDirection);
}

void UControllerPresentationComponent::HandleLockOnTargetChanged(bool bIsTargetExist)
{
	FourDirectionWidget->SetActiveStatusDirectionalUI(bIsTargetExist);
}

// Sets default values for this component's properties
UControllerPresentationComponent::UControllerPresentationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UControllerPresentationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UControllerPresentationComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UControllerPresentationComponent::CreateAndShowWidget()
{
	APlayerController* OwningController = GetOwner<APlayerController>();
	FourDirectionWidget = CreateWidget<UCombatAttackDirectionUI>(OwningController, FourDirectionWidgetClass);
	FourDirectionWidget->AddToPlayerScreen(0);
}

void UControllerPresentationComponent::BindToCharacter(ACombatCharacter* Character)
{
	Character->GetCombatStateComponent()->OnAttackDirectionChanged.AddUniqueDynamic(this, &UControllerPresentationComponent::HandleAttackDirectionChanged);
	Character->GetLockOnComponent()->OnLockOnTargetChanged.AddUniqueDynamic(this, &UControllerPresentationComponent::HandleLockOnTargetChanged);
}

