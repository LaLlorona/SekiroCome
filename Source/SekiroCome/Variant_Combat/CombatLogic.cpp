#include "CombatLogic.h"
#include "CombatLogic/FAttackData.h"
#include "CombatLogic/FDamageData.h"
#include "Interfaces/CombatAttacker.h"
#include "Interfaces/CombatDamageable.h"
#include "Interfaces/CombatParryable.h"
#include "GameFramework/Actor.h"
#include "Engine/GameInstance.h"
#include "Table/CombatDataSubsystem.h"
#include "Table/CombatDataTableManager.h"
#include "Table/Table/CombatDamageMultiplierTable.h"
#include "Table/Table/CombatDefenseDataTable.h"
#include "Table/Table/CombatWeaponDamageDataTable.h"
#include "Table/Row/FCombatDamageMultiplierDataRow.h"
#include "Table/Row/FCombatArmorDataRow.h"
#include "Table/Row/FCombatWeaponDamageDataRow.h"


namespace CombatLogic
{
	constexpr FDamageData CalculateFinalDamage(const FCombatDamageMultiplierDataRow& DamageMultiplierData, const FCombatWeaponDamageDataRow& WeaponDamageRowData, const FCombatArmorDataRow& ArmorDataRow)
	{
		// §1-2-4: For each damage type, apply a minimum of 1 (chip damage floor) to (weapon damage × attack type multiplier - defense), then sum them (minimum 3, no upper limit)
		const float ThrustContribution = FMath::Max(1.0f, (WeaponDamageRowData.ThrustDamage * DamageMultiplierData.ThrustMultiplier) - ArmorDataRow.ThrustDefense);
		const float SlashContribution = FMath::Max(1.0f, (WeaponDamageRowData.SlashDamage * DamageMultiplierData.SlashMultiplier) - ArmorDataRow.SlashDefense);
		const float BluntContribution = FMath::Max(1.0f, (WeaponDamageRowData.BluntDamage * DamageMultiplierData.BluntMultiplier) - ArmorDataRow.BluntDefense);
		auto totalDamage = ThrustContribution + SlashContribution + BluntContribution;
		auto finalDamageData = FDamageData(totalDamage * DamageMultiplierData.PriorityHealthDamageRatio, totalDamage * (1 - DamageMultiplierData.PriorityHealthDamageRatio));

		return finalDamageData;
	}

	void ResolveAttack(AActor* AttackerActor, ICombatAttacker* Attacker, AActor* DamagedActor, ICombatDamageable* Damageable, const FAttackData& AttackData)
	{
		UCombatDataTableManager* TableManager = UCombatDataSubsystem::GetCombatDataSubsystem(AttackerActor)->GetInGameTableManager();
		const FWeaponId WeaponID = Attacker->GetWeaponID();
		const FArmorTypeId ArmorTypeID = Damageable->GetArmorTypeID();

		const FCombatDamageMultiplierDataRow& DamageMultiplierData = TableManager->AttackTypeTable->FindByAttackTypeOrThrow(AttackData.AttackType);
		const FCombatWeaponDamageDataRow& WeaponDamageRowData = TableManager->WeaponDamageTable->FindByWeaponId(WeaponID);
		const FCombatArmorDataRow& ArmorDataRow = TableManager->DefenseTable->FindByArmorTypeId(ArmorTypeID);

		const FDamageData DamageData = CalculateFinalDamage(DamageMultiplierData, WeaponDamageRowData, ArmorDataRow);



		if (ICombatParryable* Parryable = Cast<ICombatParryable>(DamagedActor))
		{
			if (Parryable->CanParryNow())
			{
				if (ICombatDamageable* AttackerDamageable = Cast<ICombatDamageable>(AttackerActor))
				{
					//ToDo: Play Riposte Animation
					AttackerDamageable->ApplyDamage(DamageData, AttackerActor, AttackData.DamageLocation, AttackData.DamageImpulse);
					/*Parryable->*/
					Parryable->ChangeToRiposteState();
					return;
				}
			}
		}
		Damageable->ApplyDamage(DamageData, AttackerActor, AttackData.DamageLocation, AttackData.DamageImpulse);
	}
	EAttackDirection GetNextAttackDirectionOnAttack(const EAttackDirection CurrentAttackDirection)
	{
		switch (CurrentAttackDirection)
		{
			case EAttackDirection::Up:
				return EAttackDirection::Left;
			case EAttackDirection::Down:
				return EAttackDirection::Right;
			case EAttackDirection::Right:
				return EAttackDirection::Left;
			case EAttackDirection::Left:
				return EAttackDirection::Right;
		}
		return EAttackDirection::Left;
	}
	EAttackDirection GetNextAttackDirectionOnBlock(const EAttackDirection BlockedAttackDirection)
	{
		switch (BlockedAttackDirection)
		{
		case EAttackDirection::Up:
			return EAttackDirection::Right;
		case EAttackDirection::Down:
			return EAttackDirection::Right;
		case EAttackDirection::Right:
			return EAttackDirection::Left;
		case EAttackDirection::Left:
			return EAttackDirection::Up;
		}
		return EAttackDirection::Left;
	}

	FAttackInfoId GetAttackInfoRowNameByDirection(const EAttackDirection AttackDirection)
	{
		switch (AttackDirection)
		{
		case EAttackDirection::Up:
			return FAttackInfoId(FName("Up"));
		case EAttackDirection::Down:
			return FAttackInfoId(FName("Down"));
		case EAttackDirection::Right:
			return FAttackInfoId(FName("Right"));
		case EAttackDirection::Left:
			return FAttackInfoId(FName("Left"));
		}
		return FAttackInfoId(FName("Down"));
	}
}
