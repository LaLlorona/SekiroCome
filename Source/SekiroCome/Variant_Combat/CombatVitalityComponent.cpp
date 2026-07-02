// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatVitalityComponent.h"
#include "Table/CombatTuningDataTable.h"


// Sets default values for this component's properties
UCombatVitalityComponent::UCombatVitalityComponent()
{
	// This component is updated explicitly by its owner via CustomUpdate(), not by the engine's tick system.
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatVitalityComponent::CustomUpdate(float DeltaTime)
{
	RecomputeMaxSP();

	if (HpRegenLeftTime > 0.0f)
	{
		HpRegenLeftTime = FMath::Max(0.0f, HpRegenLeftTime - DeltaTime);
		return;
	}

	if (const FCombatTuningRow* Row = CombatTuningDataTable->FindByRowName(CombatTuningRowName))
	{
		CurrentSP = FMath::Min(CurrentSP + Row->SP_RegenPerSecond * DeltaTime, MaxSP);
	}
}

void UCombatVitalityComponent::RecomputeMaxSP()
{
	MaxSP = 30.0f + 70.0f * (MaxHP > 0.0f ? CurrentHP / MaxHP : 0.0f);
	CurrentSP = FMath::Min(CurrentSP, MaxSP);
}

void UCombatVitalityComponent::OnRegenStopTimerBegin()
{
	if (const FCombatTuningRow* Row = CombatTuningDataTable->FindByRowName(CombatTuningRowName))
	{
		HpRegenLeftTime = Row->SP_RegenDelayInSecond;
	}
}

void UCombatVitalityComponent::Initialize(UCombatTuningDataTable* InTuningDataTable, FName InTuningRowName)
{
	CombatTuningDataTable = InTuningDataTable;
	CombatTuningRowName = InTuningRowName;
	OnRegenStopTimerBegin();
}

void UCombatVitalityComponent::ResetVitality()
{
	CurrentHP = MaxHP;
	RecomputeMaxSP();
	CurrentSP = MaxSP;
}

float UCombatVitalityComponent::ApplyDamage(float Damage)
{
	if (!IsAlive())
	{
		return 0.0f;
	}
	if (CurrentSP >= Damage)
	{
		CurrentSP -= Damage;
	}
	else
	{
		auto hpDamage = Damage - CurrentSP;
		CurrentSP = 0.0f;
		CurrentHP -= hpDamage;
	}

	RecomputeMaxSP();

	return Damage;
}

float UCombatVitalityComponent::GetHPPercentage() const
{
	return MaxHP > 0.0f ? CurrentHP / MaxHP : 0.0f;
}

