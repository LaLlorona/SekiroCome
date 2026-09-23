# Implementation Checklist (based on combatsystem_base.md)

> **Completion rule**: After finishing each item, verify it in the actual code, then change `[ ]` → `[x]`.
> **Reference design doc**: `docs/combatsystem_base.md`

---

## Currently Implemented (verified against existing code)

- [x] HP basic structure (`ACombatCharacter::CurrentHP / MaxHP`)
- [x] Attack combo / charged attack montage playback
- [x] Attack collision detection (`DoAttackTrace` — sphere sweep)
- [x] Lock-on component (`UCombatLockOnComponent`)
- [x] Guard / parry state machine (Idle / Guard / PartialParry / PerfectParryRiposte)
- [x] PerfectParry timing window (within `RiposteMinimumTimeWindow` after entering Guard)
- [ ] Riposte montage playback (`UCombatMontageSet`)
- [x] Attack direction enum (`EAttackDirection`: Up / Down / Left / Right)
- [x] Movement input → direction conversion (`ECombatInputDirectionEnum`, `GetCombatInputDirection`)
- [x] Hit resolution logic (`CombatLogic::ResolveAttack` — checks for parry, then applies damage)
- [x] AnimNotify hookup (DoAttackTrace / CheckCombo / CheckChargedAttack)

---

## Phase 1 — Stamina System (Design §1)

> **Why first**: All later systems, such as damage calculation and MasterStrike counter conditions, depend on stamina.

- [x] Add `CurrentSP / MaxSP` to `ACombatCharacter` (`MaxSP = 30 + 70 * HP/100`) — implemented by creating `UCombatVitalityComponent` (`CurrentHP/MaxHP/CurrentSP/MaxSP`) owned by `ACombatCharacter` (`VitalityComponent` field)
- [x] Add the same SP fields to `ACombatEnemy` — owns `VitalityComponent` (`UCombatVitalityComponent`), calls `Initialize(CombatTuningDataTable, CombatTuningRowName)` in `BeginPlay`, explicitly calls `CustomUpdate` in `Tick`, and `TakeDamage` now routes through `VitalityComponent::ApplyDamage`
- [x] Damage handling: if SP > 0, deduct from SP first; excess carries over to HP immediately (`UCombatVitalityComponent::ApplyDamage`)
- [x] Stamina regen tick (`SP_RegenPerSecond`, handled every tick in `UCombatVitalityComponent::CustomUpdate` — called explicitly from `ACombatCharacter::Tick`, not the engine's `TickComponent`)
- [~] Regen stop conditions — reset timer (`SP_RegenDelay`) when Attack / Guard / Dodge / Jump / Sprint starts — `VitalityComponent->OnRegenStopTimerBegin()` is hooked up for player attacks (`ComboAttack`/`ChargedAttack`), guard (`TryGuardStart`), and AI attacks (`DoAIComboAttack`/`DoAIChargedAttack`). Dodge/Jump/Sprint are not applied yet because those actions don't exist in `ACombatCharacter`/`ACombatEnemy` (hook them up when those systems are implemented)
- [x] Movement (walking) excluded from regen stop conditions (`DoMove` currently has no stamina effect — keep as-is)
- [x] Immediately clamp SP max when HP changes (`UCombatVitalityComponent::RecomputeMaxSP`, called from `ApplyDamage`/`ResetVitality`/`CustomUpdate`)
- [x] Create DataTable: `UCombatTuningDataTable` (`FCombatTuningRow`: `SP_RegenPerSecond`, `SP_RegenDelayInSecond`) + dedicated `UFactory`

---

## Phase 2 — Weapon Damage Types / Formula (Design §1-2, 3-table approach)

> **Why first**: Completes the damage pipeline together with the Phase 1 stamina system.

- [x] Weapon damage table (`FCombatWeaponDamageRow`: ThrustDamage/SlashDamage/BluntDamage) + `UCombatWeaponDamageDataTable` + dedicated `UFactory` (§1-2-1)
- [x] Attack type multiplier table (`FCombatAttackTypeRow`: ThrustMultiplier/SlashMultiplier/BluntMultiplier/PriorityHealthDamageRatio) + `UCombatAttackTypeDataTable` + dedicated `UFactory` (§1-2-2, 10 rows planned: Left/Right/Up/Down attack, Riposte, combo finisher, MasterStrike)
- [x] Armor defense table (`FCombatDefenseRow`: ThrustDefense/SlashDefense/BluntDefense, RowName = armor type ID) + `UCombatDefenseDataTable` + dedicated `UFactory` (§1-2-3) — renamed from `FCombatEnemyDefenseRow`/`UCombatEnemyDefenseDataTable` so it can be shared by player and enemies (added `CoreRedirects` to `DefaultEngine.ini` for compatibility with the existing `DT_EnemyDefenseData.uasset`)
- [x] Add attack type RowName (`AttackTypeRowName`) field to `FAttackData` (weapon ID and armor type ID are each held directly by `ACombatCharacter`/`ACombatEnemy` — equipment system to be implemented later). Added interface getters `ICombatAttacker::GetWeaponID()` / `ICombatDamageable::GetArmorTypeID()`, called from `CombatLogic::ResolveAttack` to obtain the values (not yet used in the actual damage formula)
- [x] Implement the final damage formula (§1-2-4) + separate Priority HP damage application (§1-2-5) in `CombatLogic::ResolveAttack` — `CombatLogic::CalculateFinalDamage` returns `FDamageData(PriorityHealthDamage, RemainingDamage)`, and both `ICombatDamageable::ApplyDamage`/`UCombatVitalityComponent::ApplyDamage` now take `FDamageData`. `ACombatCharacter`/`ACombatEnemy` moved the actual application logic into a private helper `ApplyDamageToVitality(const FDamageData&)`, and the engine-standard `TakeDamage(float,...)` override was removed entirely since nothing in this project calls it

---

## Phase 3 — Automatic Attack Direction Switching (Design §3-1, §3-2)

> **Why first**: Prerequisite for the combo history buffer and MasterStrike difficulty branching.

- [x] Add player "next attack direction" state variable (`EAttackDirection NextAttackDirection`)
- [x] Implement automatic direction switch table after a successful attack (§3-1: Right→Left, Left→Right, Down→Right, Up→Left)
- [x] Implement automatic direction switch table after a successful guard (Block) (§3-2: Right→Left, Left→Up, Down→Right, Up→Right)
- [x] Hook up `NextAttackDirection` to `CombatAttackDirectionUI` (using the existing UI widget)

---

## Phase 4 — MasterStrike / PerfectBlock (Design §5)

> **Why first**: A judgement window system for enemy attacks; separate from the existing PerfectParry (timing based on when the player starts guarding), it needs an AnimNotify window based on the enemy attack montage.

- [ ] Add `AnimNotify_StartPerfectBlockWindow` / `AnimNotify_FinishPerfectBlockWindow` to enemy attack montages — defines the judgement window start/end (§5-1)
- [ ] Show a shield icon UI at the center of the screen during the judgement window (show when the window starts, remove when it ends) (§5-1)
- [ ] Branch player input within the judgement window: guard input triggers PerfectBlock / attack input in a MasterStrike-capable direction triggers MasterStrike (§5-1)
- [ ] On PerfectBlock, set player damage to 0 and end (§5-1)
- [ ] On MasterStrike, don't play the montage immediately on input; at the `AnimNotify_PlayMasterStrikeMontage` point in the enemy attack montage, play the player Riposte montage and enemy hit montage simultaneously (§5-1)
- [ ] Add `AnimNotify_OnGetRipostedByMasterStrike` to the enemy hit montage — apply damage to the enemy at this point (§5-1)
- [ ] Add `AnimNotify_OnSuccessMasterStrike` to the player Riposte montage — recover 10 player stamina at this point (§5-1)
- [ ] MasterStrike damage reuses the unified §1-2-4/1-2-5 damage formula without a separate formula — only needs hooking up to the "MasterStrike" row of `UCombatAttackTypeDataTable` (table already prepared in Phase 2) (§5-2)
- [ ] Add per-weapon judgement window timing DataTable constants: `MasterStrike_WindowStart` / `MasterStrike_WindowEnd` / `MasterStrike_ColliderActivation` (InGameTime-based, Time Dilation corrected) (§5-1, §7)

---

## Phase 5 — Combo Command Matching System (Design §4)

> **Why first**: The history buffer is only meaningful after automatic direction switching (Phase 3) is done.

- [ ] Add attack history buffer structure (`TArray<EAttackDirection> AttackHistoryBuffer`, max length 3)
- [ ] On successful attack, append the direction to the buffer + check for pattern matches
- [ ] Implement buffer reset conditions (getting PerfectBlocked / exceeding `Combo_BufferTimeout`)
- [ ] DataTable-based combo pattern definitions (`FCombatComboRow`: pattern array + finisher montage + damage value)
- [ ] Register the 3 basic patterns (Left→Right→Left / Right→Left→Right / Down→Down→Right)
- [ ] Play finisher montage on successful pattern match
- [ ] Combo finisher damage: apply the §1-2 unified formula using the "ComboFinisher" row of `UCombatAttackTypeDataTable` — §4-5
- [ ] Reset combo buffer when PerfectBlocked (finalized rule §3-5)
- [ ] Enemy counterattack condition on successful enemy PerfectBlock (`Enemy_CounterStaminaThreshold` DataTable constant)

---

## Phase 6 — Movement System (Design §2)

> **Why first**: Lock-on-linked movement to complete the combat feel.

- [ ] Implement Left/Right input → circular strafe movement while locked on (`LockOn_StrafeRadius`, `LockOn_AngularSpeed`)
- [ ] Add strafe radius / angular speed DataTable
- [ ] AI multi-enemy surround slot system (`Surround_MaxCloseSlots`, `Surround_CloseSlotRadius`, `Surround_SideAngle`)

---

## Full DataTable Constant List (Design §7)

> Independent of whether the Phases above are complete, move any hardcoded values into a DataTable.

- [ ] `SP_RegenPerSecond`
- [ ] `SP_RegenDelay`
- [ ] `LockOn_StrafeRadius`
- [ ] `LockOn_AngularSpeed`
- [ ] `Surround_MaxCloseSlots`
- [ ] `Surround_CloseSlotRadius`
- [ ] `Surround_SideAngle`
- [ ] `Enemy_CounterStaminaThreshold`
- [ ] `Combo_BufferTimeout`
- [ ] `Combo_InputWindow`
- [ ] `MasterStrike_WindowStart` / `MasterStrike_WindowEnd` / `MasterStrike_ColliderActivation` (per weapon)
- [ ] `PerfectBlock_CounterWindow`
- [ ] `PerfectBlock_GuaranteedHit`
- [x] Per-weapon Thrust/Slash/Blunt damage (`UCombatWeaponDamageDataTable`, §1-2-1)
- [x] Per-attack-type (Left/Right/Up/Down attack, Riposte, combo finisher, MasterStrike) Thrust/Slash/Blunt multipliers + Priority HP Ratio (`UCombatAttackTypeDataTable`, §1-2-2)
- [x] Per-armor-type Thrust/Slash/Blunt defense (`UCombatDefenseDataTable`, §1-2-3, shared by player and enemies)
