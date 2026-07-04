// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatDataSubsystem.h"
#include "CombatDataTableManager.h"


void UCombatDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	
	Super::Initialize(Collection);
	InGameTableManager = LoadObject<UCombatDataTableManager>(nullptr, TEXT("/Game/Variant_Combat/Data/DA_CombatDataManager.DA_CombatDataManager"));

	if (!InGameTableManager)
	{
		UE_LOG(LogTemp, Error, TEXT("UCombatDataSubsystem: failed to load DA_CombatData"));
	}
}

UCombatDataSubsystem* UCombatDataSubsystem::GetCombatDataSubsystem(const UObject* WorldContextObject)
{
	const UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
	const UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
	return GameInstance ? GameInstance->GetSubsystem<UCombatDataSubsystem>() : nullptr;
}
