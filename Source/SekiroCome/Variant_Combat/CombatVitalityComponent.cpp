// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatVitalityComponent.h"
#include "Table/CombatDataTableManager.h"
#include "Table/CombatDataSubsystem.h"
#include "Table/Table/CombatTuningDataTable.h"


// Sets default values for this component's properties
UCombatVitalityComponent::UCombatVitalityComponent()
{
	// This component is updated explicitly by its owner via CustomUpdate(), not by the engine's tick system.
	PrimaryComponentTick.bCanEverTick = false;

	// start fully alive by default, so anything reading HP/SP before Initialize() runs (e.g. StateTree on possess) sees a valid state
	CurrentHP = MaxHP;
	CurrentSP = MaxSP;
}

void UCombatVitalityComponent::CustomUpdate(float DeltaTime)
{
	CurrentSP = FMath::Min(CurrentSP, GetCurrentPossibleMaxSP());
	if (HpRegenLeftTime > 0.0f)
	{
		HpRegenLeftTime -= DeltaTime ;
		return;
	}

	const FCombatTuningRow& Row = UCombatDataSubsystem::GetCombatDataSubsystem(this)->GetInGameTableManager()->CombatTuningDataTable->FindByCombatTuningId(CombatTuningRowName);
	CurrentSP = FMath::Min(CurrentSP + Row.SP_RegenPerSecond * DeltaTime, GetCurrentPossibleMaxSP());
}

void UCombatVitalityComponent::OnRegenStopTimerBegin()
{
	const FCombatTuningRow& Row =
		UCombatDataSubsystem::GetCombatDataSubsystem(this)->GetInGameTableManager()->CombatTuningDataTable->FindByCombatTuningId(CombatTuningRowName);
	HpRegenLeftTime = Row.SP_RegenDelayInSecond;
}

void UCombatVitalityComponent::Initialize(FCombatTuningId InTuningRowName)
{
	CombatTuningRowName = InTuningRowName;
	OnRegenStopTimerBegin();
}

void UCombatVitalityComponent::ResetVitality()
{
	CurrentHP = MaxHP;
	CurrentSP = GetCurrentPossibleMaxSP();
}

float UCombatVitalityComponent::ApplyDamage(const FDamageData& DamageData)
{
	if (!IsAlive())
	{
		return 0.0f;
	}

	// §1-2-5: bypasses stamina entirely, applied straight to HP
	CurrentHP -= DamageData.PriorityHealthDamage;

	// §1-3: stamina-first, overflow to HP
	const float Damage = DamageData.RemainingDamage;
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

	CurrentSP = FMath::Min(CurrentSP, GetCurrentPossibleMaxSP());
	OnRegenStopTimerBegin();

	return DamageData.PriorityHealthDamage + DamageData.RemainingDamage;
}

float UCombatVitalityComponent::GetHPPercentage() const
{
	return MaxHP > 0.0f ? CurrentHP / MaxHP : 0.0f;
}

float UCombatVitalityComponent::GetStaminaPercentage() const
{
	return MaxSP >= 0.0f ? CurrentSP / MaxSP : 0.0f;
}

float UCombatVitalityComponent::GetCurrentPossibleMaxSP() const
{
	return 30.0f + 70.0f * (MaxHP > 0.0f ? CurrentHP / MaxHP : 0.0f);
}

