// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatLockOnComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UCombatLockOnComponent::UCombatLockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCombatLockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatLockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FLockOnResult UCombatLockOnComponent::GetLockOnResult() const
{
	if (LockedOnTarget.IsValid())
	{
		return FLockOnResult(TInPlaceType<FLockOnTargetExist>{}, FLockOnTargetExist{ LockedOnTarget->GetActorLocation() });
	}
	return FLockOnResult(TInPlaceType<FLockOnTargetNotExist>{});
}

void UCombatLockOnComponent::TryLockOn(FVector CameraLocation, FVector CameraForward)
{
	if (LockedOnTarget.IsValid())
	{
		LockedOnTarget = nullptr;
		return;
	}
	TArray<AActor*> OutActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	UKismetSystemLibrary::SphereOverlapActors(this, CameraForward, LockOnTargetSearchDistance, ObjectTypes, nullptr, {GetOwner()}, OutActors);

	if (OutActors.Num() > 0)
	{
		LockedOnTarget = OutActors[0];
		
	}
	for (auto Actor : OutActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found actor: %s"), *Actor->GetName());
	}
	
}

