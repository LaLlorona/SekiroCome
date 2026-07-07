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
#include "Table/CombatDamageMultiplierTable.h"
#include "Table/CombatDefenseDataTable.h"
#include "Table/CombatWeaponDamageDataTable.h"
#include "Table/FCombatDamageMultiplierDataRow.h"
#include "Table/FCombatArmorDataRow.h"
#include "Table/FCombatWeaponDamageDataRow.h"


namespace CombatLogic
{
	constexpr FDamageData CalculateFinalDamage(const FCombatDamageMultiplierDataRow& DamageMultiplierData, const FCombatWeaponDamageDataRow& WeaponDamageRowData, const FCombatArmorDataRow& ArmorDataRow)
	{
		// §1-2-4: 속성별 (무기 데미지 × 공격 종류 배율 - 방어력)에 최소 1 보장(chip damage floor)을 적용한 뒤 합산 (최소 3, 상한 없음)
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
		const FName WeaponID = Attacker->GetWeaponID();
		const FName ArmorTypeID = Damageable->GetArmorTypeID();

		const FCombatDamageMultiplierDataRow& DamageMultiplierData = TableManager->AttackTypeTable->FindByAttackTypeOrThrow(AttackData.AttackType);
		const FCombatWeaponDamageDataRow& WeaponDamageRowData = TableManager->WeaponDamageTable->FindByRowNameOrThrow(WeaponID);
		const FCombatArmorDataRow& ArmorDataRow = TableManager->DefenseTable->FindByRowNameOrThrow(ArmorTypeID);

		const FDamageData DamageData = CalculateFinalDamage(DamageMultiplierData, WeaponDamageRowData, ArmorDataRow);



		if (ICombatParryable* Parryable = Cast<ICombatParryable>(DamagedActor))
		{
			if (Parryable->CanParryNow())
			{
				if (ICombatDamageable* AttackerDamageable = Cast<ICombatDamageable>(AttackerActor))
				{
					//ToDo: Riposte Animation 재생
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

	FName GetAnimationSectionNameByAttackDirection(const EAttackDirection AttackDirection)
	{
		switch (AttackDirection)
		{
		case EAttackDirection::Up:
			//ToDO: 지금 AnimMontage 에 UpAttack 이 없어서 일단 Down 으로 설정
			return FName("Down");
		case EAttackDirection::Down:
			return FName("Down");
		case EAttackDirection::Right:
			return FName("Right");
		case EAttackDirection::Left:
			return FName("Left");
		}
		return FName("Down");
	}
}
