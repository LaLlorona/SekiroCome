# SekiroCome

A 3D melee action prototype in **Unreal Engine 5.7 (C++)**, built around a skill-expressive 1v1 combat system based on **directional attacks and parrying**, inspired by the parry-centric feel of *Kingdom Come: Deliverance II*.

---

## Overview

| | |
|---|---|
| **Engine** | Unreal Engine 5.7 |
| **Language** | C++ (gameplay logic), Blueprint (asset wiring / UI) |
| **Genre** | 3D melee action, 1v1 combat |
| **Base** | Started from the UE5 Third Person template (Combat variant); the combat systems listed below were designed and implemented on top of it |
| **Status** | In development |

The core idea: every attack has one of **four directions (Up / Down / Left / Right)**. The next direction switches automatically after each attack or block, following fixed tables. Players can also re-aim with a mouse flick. The switch tables are intentionally asymmetric, so some directions are "easy" and others "hard" when setting up a counter (MasterStrike), which makes direction choice a skill expression.

---

## Project Structure

```
Source/SekiroCome/Variant_Combat/
├── CombatCharacter / CombatVitalityComponent / CombatLockOnComponent
├── CombatLogic.*           # Stateless combat rules (damage, direction tables, hit resolution)
├── CombatLogic/            # FAttackData, FDamageData, constants
├── Player/
│   ├── State/              # Combat state machine + state classes
│   ├── PlayerComponent/    # Input component, mouse flick detection
│   └── PlayerController/   # Presentation (HUD) component
├── Table/
│   ├── Id/                 # Strongly typed row keys
│   ├── Row/                # Row structs
│   ├── Table/              # Typed UDataTable subclasses
│   ├── TableFactory/       # Editor-only UFactory per table
│   └── CombatDataSubsystem / CombatDataTableManager
├── Animation/              # Custom AnimNotifies (timing windows, attack trace)
├── Interfaces/             # ICombatAttacker, ICombatDamageable, ICombatParryable
├── UI/                     # Attack direction HUD, life/stamina bar
└── AI/, Gameplay/          # Mostly from the UE5 Combat template
docs/
├── combatsystem_base.md          # Combat design specification
└── implementation_checklist.md   # Phased implementation tracking
```

---

## Implemented Features

### Combat State Machine
- Player combat logic runs on an interface-based state machine (`IPlayerCombatState`) with **Idle / Attack / Guard / PartialParry / PerfectParryRiposte / Hit** states.
- Each state owns its enter/update/exit logic and returns its own transition through `GetStateToTransition()`, so the state machine component does not need a central `switch` statement.
- The animation layer reads the current state via `EAnimationStateEnum`, keeping gameplay state and animation state decoupled.

### Directional Attack System
- 4-direction attacks, with the next direction auto-switched after an attack or a block via fixed tables (`CombatLogic::GetNextAttackDirectionOnAttack / OnBlock`).
- While locked on, **mouse flicks** re-aim the attack direction. `UMouseInputThresholdCheckComponent` detects flicks with a *leaky accumulator* (per-frame mouse delta accumulated and decayed over time), so no click-and-drag is needed.
- If the player re-aims manually during an attack, the automatic switch for that attack is suppressed.
- A HUD widget shows the prepared direction and is driven by a delegate (`OnAttackDirectionChanged`) rather than polling.

### Attack Input Buffering (AnimNotify-Driven Timing)
- On entering the Attack state, the montage section's AnimNotifies are scanned once and their trigger times cached, corrected for play rate:
  - `AnimNotify_AttackInputWindowOpened`: when input starts being accepted
  - `AnimNotify_AttackTransitionWindowOpened`: when a buffered input may chain into the next attack
  - `AnimNotify_AttackDirectionChange`: when the direction auto-switches
- Buffered inputs are consumed only once the transition window opens, so combo timing is authored per animation in the editor instead of hardcoded.

### Guard / Parry / Riposte
- If an attack lands within a short window after the defender starts guarding, it counts as a **perfect parry**. The damage is reflected back to the attacker, and the defender enters the Riposte state. Riposte montage playback is still in progress.
- Parry resolution goes through small interfaces (`ICombatAttacker`, `ICombatDamageable`, `ICombatParryable`), so players and enemies share the same hit pipeline (`CombatLogic::ResolveAttack`).

### HP / Stamina (Vitality) System
- `UCombatVitalityComponent` is shared by both the player and enemies.
- **Stamina-first damage with overflow**: damage hits stamina first, and any excess carries over to HP within the same hit.
- **Priority HP damage**: a per-attack-type ratio of damage bypasses stamina and hits HP directly.
- Max stamina scales with HP (`30 + 70 * HP / 100`) and is clamped immediately when HP drops.
- Stamina regen pauses for a tunable delay after attacking or guarding; walking does not block regen.

### Damage Formula (3-Table System)
- Final damage combines three DataTables: **weapon damage** (Thrust / Slash / Blunt) × **attack type multiplier** − **armor defense**, with a minimum of 1 chip damage per damage type.
- The result is split into `FDamageData(PriorityHealthDamage, RemainingDamage)` and applied through the vitality component.

### Lock-On
- Sphere-overlap target acquisition and a camera that tracks the locked-on target.
- Lock-on results are returned as a `TVariant` (target exists / no target), so callers must handle both cases explicitly.
- Lock-on change events are broadcast only on frames where the target state actually changes.

---

## Architecture Highlights

### Data-Driven Design with Typed DataTables
All tunable values (damage, multipliers, defense, stamina regen, montages, attack info) live in DataTables, not in code.

- **One `UDataTable` subclass per row struct** (e.g. `UCombatWeaponDamageDataTable`), with `RowStruct` locked in the constructor. The editor asset picker then only shows tables with the right schema.
- **Typed lookup methods** (e.g. `FindByWeaponId`) instead of the generic `FindRow<T>()`.
- **Strongly typed row keys** (`FWeaponId`, `FArmorTypeId`, `FAttackInfoId`, `FMontageId`, `FCombatTuningId`): each wraps an `FName`, so passing a key to the wrong table is a compile error.
- **Custom `UFactory` per table type**, so each table can be created directly from the Content Browser without the row-struct picker. It overrides `MakeNewDataTable()` because the base `UDataTableFactory` always constructs a plain `UDataTable`. Factories are editor-only (`#if WITH_EDITOR`, conditional `UnrealEd` dependency) so packaged builds are unaffected.
- All tables are grouped in a `UCombatDataTableManager` data asset and served through a `UGameInstanceSubsystem` (`UCombatDataSubsystem`).

### Explicit Component Update Order
Owned components such as `UCombatVitalityComponent` and `UCombatLockOnComponent` use a `CustomUpdate(DeltaTime)` method called from the owner's `Tick()`, instead of relying on `TickComponent`. This makes the per-frame update order explicit:

```
Lock-on update → attack direction input → combat state machine → vitality → UI
```

### Separation of Concerns
- **Input** (`UCombatCharacterInputComponent`): Enhanced Input bindings, mouse flick detection
- **Presentation** (`UControllerPresentationComponent`): HUD widgets, bound to gameplay through delegates
- **Rules** (`CombatLogic` namespace): stateless combat rules (damage formula, direction tables, hit resolution)
- **State** (`UPlayerCombatStateMachineComponent` + state objects): what the character is currently doing

---

## Development Process & Use of AI

This project uses a **design-document-driven workflow**, with AI (Claude) involved at specific stages:

1. **Design (AI-assisted)**: I worked with AI to turn my combat ideas into a structured design specification ([`docs/combatsystem_base.md`](docs/combatsystem_base.md)). It defines the rules, formulas, edge cases, and a list of every tunable constant.
2. **Planning**: The spec was broken down into a phased checklist ([`docs/implementation_checklist.md`](docs/implementation_checklist.md)). An item is marked done only after the feature is verified in the actual code.
3. **Implementation (by me)**: I implemented the systems in C++ from the spec. The AI was set up as a *tutor* rather than a code generator ([`CLAUDE.md`](CLAUDE.md)). By default it only describes what needs to be built, gives step-by-step hints when I'm stuck, and provides full code only when I explicitly ask. I also used it for code review and for debugging engine-level issues.
4. **Conventions**: Engineering conventions found along the way (typed DataTables, custom factories, explicit `CustomUpdate`) are recorded in `CLAUDE.md`, so they are applied consistently across the codebase.

The goal was to use AI to **sharpen the design and speed up learning Unreal Engine**, while keeping the implementation and architectural decisions my own.

---

## Roadmap

Tracked in [`docs/implementation_checklist.md`](docs/implementation_checklist.md).

- [x] Stamina / HP system and 3-table damage pipeline
- [x] Directional attacks with automatic direction switching
- [x] Attack input buffering via AnimNotify timing
- [ ] Riposte / partial parry montage playback
- [ ] **MasterStrike / PerfectBlock**: enemy-attack-based judgement window (AnimNotify window markers are in place), synchronized riposte montages
- [ ] **Combo command matching**: fighting-game-style input-history buffer matched against DataTable-registered patterns (e.g. Down → Down → Right = Pommel Strike)
- [ ] Stamina regen stop on Dodge / Jump / Sprint (actions not yet implemented)
- [ ] Lock-on circular strafe and multi-enemy surround slot AI

---

## Building

1. Install **Unreal Engine 5.7**.
2. Right-click `SekiroCome.uproject` → *Generate Visual Studio project files*.
3. Open `SekiroCome.sln` and build the `SekiroCome Editor` / `Development Editor` target, or open the `.uproject` directly and let the editor compile.
