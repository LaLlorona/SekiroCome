// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatVitalityComponent.generated.h"

class UCombatTuningDataTable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SEKIROCOME_API UCombatVitalityComponent : public UActorComponent
{
	GENERATED_BODY()
	float HpRegenLeftTime = 0.0f;

	UPROPERTY()
	TObjectPtr<UCombatTuningDataTable> CombatTuningDataTable;

	FName CombatTuningRowName;

	/** Recomputes MaxSP from the current HP ratio and clamps CurrentSP to it */
	void RecomputeMaxSP();


public:
	// Sets default values for this component's properties
	UCombatVitalityComponent();

	/** Max amount of HP the owner will have on reset */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Damage", meta = (ClampMin = 0, ClampMax = 100))
	float MaxHP = 100.0f;

	/** Current amount of HP the owner has */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Damage")
	float CurrentHP = 0.0f;

	/** MaxSP = 30 + 70 * (CurrentHP / MaxHP), recomputed whenever HP changes */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Damage")
	float MaxSP = 100.0f;

	/** Current amount of HP the owner has */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Damage")
	float CurrentSP = 0.0f;




	

	

	/** Resets CurrentHP to MaxHP */
	void ResetVitality();

	/** Reduces CurrentHP by Damage (not clamped, mirrors previous TakeDamage behavior). Returns the amount applied, or 0 if already dead */
	float ApplyDamage(float Damage);

	/** True while CurrentHP is above zero */
	bool IsAlive() const { return CurrentHP > 0.0f; }

	/** CurrentHP / MaxHP, safe against MaxHP == 0 */
	float GetHPPercentage() const;

	/** Called explicitly by the owner (e.g. from its Tick), instead of relying on this component's own TickComponent */
	void CustomUpdate(float DeltaTime);

	void OnRegenStopTimerBegin();

	void Initialize(UCombatTuningDataTable* InTuningDataTable, FName InTuningRowName);
};
