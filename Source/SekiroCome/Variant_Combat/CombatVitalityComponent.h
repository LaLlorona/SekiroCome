// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatLogic/FDamageData.h"
#include "Table/Id/FCombatTuningId.h"
#include "CombatVitalityComponent.generated.h"

class UCombatTuningDataTable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SEKIROCOME_API UCombatVitalityComponent : public UActorComponent
{
	GENERATED_BODY()
	float HpRegenLeftTime = 0.0f;

	FCombatTuningId CombatTuningRowName;

public:
	// Sets default values for this component's properties
	UCombatVitalityComponent();

	/** Max amount of HP the owner will have on reset */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Damage", meta = (ClampMin = 0, ClampMax = 100))
	float MaxHP = 100.0f;

	/** Current amount of HP the owner has */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Damage")
	float CurrentHP = 0.0f;

	/** Absolute SP scale (fixed at 100, same as MaxHP) — used as the denominator for the stamina bar percentage. The actual usable cap at any moment is GetCurrentPossibleMaxSP() */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Damage")
	float MaxSP = 100.0f;

	/** Current amount of HP the owner has */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Damage")
	float CurrentSP = 0.0f;




	

	

	/** Resets CurrentHP to MaxHP */
	void ResetVitality();

	/** Applies PriorityHealthDamage directly to CurrentHP, then RemainingDamage via the stamina-first/overflow rule. Returns the total amount applied, or 0 if already dead */
	float ApplyDamage(const FDamageData& DamageData);

	/** True while CurrentHP is above zero */
	bool IsAlive() const { return CurrentHP > 0.0f; }

	/** CurrentHP / MaxHP, safe against MaxHP == 0 */
	float GetHPPercentage() const;

	float GetStaminaPercentage() const;

	/** The usable SP cap right now: 30 + 70 * (CurrentHP / MaxHP). Computed on demand, never cached */
	float GetCurrentPossibleMaxSP() const;

	/** Called explicitly by the owner (e.g. from its Tick), instead of relying on this component's own TickComponent */
	void CustomUpdate(float DeltaTime);

	void OnRegenStopTimerBegin();

	void Initialize(FCombatTuningId InTuningRowName);
};
