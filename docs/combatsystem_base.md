# Combat System


---
## 0. Player Actions

The player can perform the following four actions.

- **Movement** (moving)
- **Attack**
- **Guard** (defense)
- **Dodge**
  
All later systems (stamina regen stop conditions, combo buffer keep/reset, judgement priority, etc.) are defined in terms of these four actions.


## 1. HP / Stamina System

### 1-1. Basic Structure
- Both the player and enemies have HP and Stamina (SP). Each has a maximum of 100.
- Each weapon has three separate damage values: Slash / Thrust / Blunt.

### 1-2. Damage Calculation (3-table approach, finalized)

Damage is calculated by combining the three DataTables below. This formula is **the basis for all damage calculation** — not only Left/Right/Up/Down basic attacks and Riposte, but also combo finishers and MasterStrike. (The former sections 4-5 and 5-4, which were dedicated to combo finisher / MasterStrike damage, have been merged into this section and removed.)

#### 1-2-1. Weapon Damage Table
Records the base attack power of each weapon for the three damage types: Thrust / Slash / Blunt.

| Weapon | Thrust | Slash | Blunt |
|---|---|---|---|
| (Weapon A) | value | value | value |
| (Weapon B) | value | value | value |

#### 1-2-2. Attack Type Multiplier Table
Each attack type has its own multiplier for each of the three damage types (Thrust / Slash / Blunt), and also specifies a "Priority HP Ratio" (1-2-5) per attack type. In addition to the 8 rows for Left/Right/Up/Down basic attacks and Left/Right/Up/Down Riposte, **combo finisher and MasterStrike are also registered as dedicated rows** (fixed values, independent of pattern/weapon).

| Attack Type | Thrust Multiplier | Slash Multiplier | Blunt Multiplier | Priority HP Ratio |
|---|---|---|---|---|
| Left Attack | value | value | value | value (0~1) |
| Right Attack | value | value | value | value (0~1) |
| Up Attack | value | value | value | value (0~1) |
| Down Attack | value | value | value | value (0~1) |
| Left Riposte | value | value | value | value (0~1) |
| Right Riposte | value | value | value | value (0~1) |
| Up Riposte | value | value | value | value (0~1) |
| Down Riposte | value | value | value | value (0~1) |
| Combo Finisher | value | value | value | value (0~1) |
| MasterStrike | value | value | value | value (0~1) |

#### 1-2-3. Enemy Defense Table
Records each enemy type's defense against the three damage types: Thrust / Slash / Blunt. Armor-part distinctions (head/torso/arms/legs, etc.) are skipped in the first implementation, starting with "a single defense set per enemy".

| Enemy Type | Thrust Defense | Slash Defense | Blunt Defense |
|---|---|---|---|
| (Enemy Type A) | value | value | value |

> **Design premise**: Defense is not a ratio (0~1) but **a flat reduction value (subtraction) on the same scale as weapon damage-type damage**. If defense is greater than `weapon damage-type damage × multiplier`, the subtraction itself can go negative, but the minimum-of-1 guarantee (chip damage floor) in 1-2-4 ensures that damage type's contribution is always treated as at least 1.

#### 1-2-4. Final Damage Formula

```
Final Damage = Σ over (Thrust, Slash, Blunt):
    max(1, (weapon's damage for that type × attack type's multiplier for that type) - enemy's defense for that type)
```

- Each damage type's (Thrust/Slash/Blunt) contribution is **guaranteed to be at least 1** — no matter how high the defense, a minimum amount of "chip damage" always goes through. There is no upper limit, and the minimum final damage after summing all three types is **3**.

#### 1-2-5. Applying Priority HP Damage
The portion given by the **Priority HP Ratio** specified per attack type in 1-2-2 is applied to HP immediately regardless of remaining stamina; the rest follows the stamina-first rule in 1-3 (deduct from stamina first, overflow goes to HP in the same hit).

```
Priority HP Damage = Final Damage × Priority HP Ratio        # Independent of stamina, applied to HP immediately
Remaining Damage   = Final Damage × (1 - Priority HP Ratio)  # Follows 1-3 rules (stamina first, overflow to HP)
```

**Example**: Final damage 30, Priority HP Ratio 0.3 → Priority HP Damage = 30 × 0.3 = **9** (applied to HP immediately), Remaining Damage = 30 × 0.7 = **21** (deducted from stamina first per 1-3; if stamina is insufficient, only the excess overflows to HP).

### 1-3. Damage Priority and Overflow
- While stamina > 0, damage goes to stamina first.
- **Overflow carries over.** If a hit's damage exceeds the remaining stamina, the excess immediately carries over to HP within the same hit.

```
if (remaining stamina > 0):
    staminaDeduction = min(damage, remainingStamina)
    hpDeduction = damage - staminaDeduction      # Overflow, applied immediately in the same hit
    stamina -= staminaDeduction
    hp -= hpDeduction
else:
    hp -= damage
```

### 1-4. Stamina Regen
- Max formula: `30 + 70 * (current HP / 100)` — unchanged.
- **Regen amount is a constant per second** (`SP_RegenPerSecond`, DataTable). It starts as a fixed value, not tied to HP ratio.
- **Regen stop conditions**: When performing Jump / Run (Sprint) / Dodge / Attack / Guard (holding guard), regen stops for `SP_RegenDelay` (N seconds, constant) after the action ends.
- **Movement (walking) does not block regen.**
- **Lowering the max cap**: When HP drops and the stamina max decreases, if current stamina exceeds the new max it is **immediately clamped to the new max**. (Not a gradual decrease — applied immediately.)

```
newCap = 30 + 70 * (hp / 100)
stamina = min(stamina, newCap)   # Check every frame / every event whenever HP is reduced
```

---

## 2. Movement System

### 2-1. Lock-On Strafe
- When locked on to an enemy, Left/Right input moves the player in a circle around that enemy while keeping distance.
- **Radius and angular speed are DataTable constants** (`LockOn_StrafeRadius`, `LockOn_AngularSpeed`). It's recommended to key them by weapon/equipment ID so different values can be mapped per weapon type or worn equipment (heavy armor, etc.).

### 2-2. Multi-Enemy Surround AI (Slot System)
- The number of enemies allowed close to the lock-on target, the surround angle, and the judgement radius are all **DataTable constants**.
  - `Surround_MaxCloseSlots` (number of enemies allowed close, including the lock-on target)
  - `Surround_CloseSlotRadius` (close slot judgement radius)
  - `Surround_SideAngle` (angle enemies spread out to the sides when surrounding)
- Slot occupy/release logic (e.g., recalculating slots when the lock-on target changes) is recommended to be covered in a separate AI behavior document. This document only defines the constants.

---

## 3. Attack Direction System

### 3-1. Automatic Next Direction on Successful Attack (unchanged)

| This Attack Direction | Next Auto Direction |
|---|---|
| Right | Left |
| Left | Right |
| Down | Right |
| Up | Left |

### 3-2. Automatic Next Attack Direction on Successful Block (unchanged)

| Blocked Attack Direction | Next Attack Direction |
|---|---|
| Right | Left |
| Left | Up |
| Down | Right |
| Up | Right |

### 3-3. Asymmetry Is Intentional — MasterStrike Difficulty by Direction

**Finalized**: The asymmetry in the 3-1/3-2 tables is intentional; it exists to vary the difficulty of meeting MasterStrike conditions by direction.

- When attacking **Down or Right** → simply following the automatic flow in these tables (without any extra mouse input) naturally leads into a MasterStrike-ready state. **"Easy directions"**.
- When attacking **Up or Left** → the automatic flow alone does not satisfy MasterStrike conditions; the player must **actively move the mouse to re-aim the direction** to reach a MasterStrike-ready state. **"Hard directions"**.


### 3-4. Enemy Counterattack Conditions
- If enemy stamina is at or above `Enemy_CounterStaminaThreshold` (%, DataTable constant) and a Perfect Block succeeds → the enemy counterattacks immediately from the blocked direction.
- Below this constant, it does not counterattack immediately.

### 3-5. Combo Continuation Conditions (finalized)
- **Getting Perfect Blocked breaks the combo (input sequence).**
- **A regular Block (not perfect) or a landed attack keeps the combo going.**

---

## 4. Combo System — Command Matching

### 4-1. Based on Command (Input History) Matching

 This is **the command recognition approach used in fighting games**.

- The system continuously tracks **the order of the player's recent successful attack directions (input history buffer)**.
- Each time an attack lands (or is judged valid), it checks whether **the end of the history buffer matches a registered combo pattern**.
- If it matches, this attack is replaced by **that combo's finisher** instead of a regular directional attack.

**Example**: If the player has already input "Down → Down" and inputs "Right" as the third attack → the history becomes `[Down, Down, Right]`, which matches the "Pommel Strike" pattern, so the third attack triggers as **Pommel Strike instead of a regular Right attack**.

- In this approach, **it doesn't matter whether a direction came from auto-switching or manual switching**. Only the resulting sequence of directions is considered.


| Combo Name | Pattern (Order) | Notes |
|---|---|---|
| Basic Combo 1 | Left → Right → Left | Possible with auto-switching alone |
| Basic Combo 2 | Right → Left → Right | Possible with auto-switching alone |
| Pommel Strike | Down → Down → Right | Second "Down" requires manual re-aim |

- Designed so that adding a new combo only requires registering its pattern in this dictionary (a DataTable-based `FCombatComboRow` is recommended: pattern array + finisher montage + damage value).

### 4-3. History Buffer Rules
- The buffer's max length is managed based on the longest registered combo pattern (currently 3).
- **Buffer reset conditions**:
  - Enemy succeeds a Perfect Block (same as the finalized rule in 3-5)
  - Combo timeout: reset if there is no next attack within `Combo_BufferTimeout` (seconds, DataTable constant or AnimNotify) after the last attack
- **Buffer keep conditions**: The buffer is kept when the attack is regular-Blocked or when it lands.

### 4-4. Input Grace Period Between Combo Hits
- The input grace period is specified by **a DataTable constant (`Combo_InputWindow`) or an AnimNotify on the attack animation**. (AnimNotify is recommended, since timing can differ per montage.)

### 4-5. Finisher Damage (merged into 1-2)

Combo finisher damage uses the unified damage formula in 1-2-4/1-2-5 as-is. The attack type multipliers and Priority HP Ratio come from the "Combo Finisher" row in the 1-2-2 table; there are no separate per-pattern damage values.

---

## 5. MasterStrike / PerfectBlock

### 5-1. Judgement Window (linked to 3-3 rules)
- During an enemy attack motion, there is a start and end point for the window in which MasterStrike / PerfectBlock can be input.
- These are defined by AnimNotify_StartPerfectBlockWindow and AnimNotify_FinishPerfectBlockWindow.
- During this window, a shield icon appears at the center of the screen.
- If the player presses guard, or attacks from a direction where MasterStrike is possible, during this window, PerfectBlock or MasterStrike is triggered.
- When Perfect Block is triggered, it ends with the player taking no damage.
- When Master Strike is triggered, depending on the player's Master Strike direction, the player plays a Riposte Montage and the enemy plays a Montage of being hit by the Master Strike.
- When Master Strike is triggered, the Montages don't play immediately; the player and enemy play their Montages simultaneously at the AnimNotify_PlayMasterStrikeMontage point in the enemy's attack motion.
- When Master Strike is triggered, those Montages contain AnimNotify_OnGetRipostedByMasterStrike and AnimNotify_OnSuccessMasterStrike, so
- The enemy hit by the Master Strike takes damage at OnGetRespostedByMasterStrike, and the player who succeeded the Master Strike recovers 10 stamina at AnimNotify_OnSuccessMasterStrike.
```
Timeline (example, based on InGameTime — per-weapon constants)
0.0s   Enemy attack starts
0.1s   Shield icon appears (judgement window starts) — per-weapon constant
0.5s   Judgement window ends — per-weapon constant
0.6s   On MasterStrike success, montage starts (independent of input timing)
0.8s   On MasterStrike failure, same as regular enemy attack logic.
```

### 5-2. MasterStrike Damage (merged into 1-2)

MasterStrike damage also uses the unified damage formula in 1-2-4/1-2-5 as-is. The attack type multipliers and Priority HP Ratio come from the "MasterStrike" row in the 1-2-2 table.


---

## 6. Common Rules During Movement/Attack
- Walking (movement) does not block stamina regen (1-4).
- Jump/Sprint/Dodge/Attack/Guard delay stamina regen (1-4).

---

## 7. DataTable Constant List (design finalized, values TBD)

When implementing, do not hardcode these in code; expose the items below through a DataTable (or CurveTable).

**Stamina / HP**
- `SP_RegenPerSecond`
- `SP_RegenDelay` (stop duration after Jump/Sprint/Dodge/Attack/Guard)

**Movement**
- `LockOn_StrafeRadius`
- `LockOn_AngularSpeed`
- `Surround_MaxCloseSlots`
- `Surround_CloseSlotRadius`
- `Surround_SideAngle`

**Combat Judgement**
- `Enemy_CounterStaminaThreshold` (%)
- `Combo_BufferTimeout`
- `Combo_InputWindow` (or replaced by AnimNotify)

**MasterStrike (individual values per weapon)**
- `MasterStrike_WindowStart` / `MasterStrike_WindowEnd`

**PerfectBlock**
- `PerfectBlock_CounterWindow`
- `PerfectBlock_GuaranteedHit` (bool)

**Damage Calculation (1-2)**
- Per-weapon Thrust/Slash/Blunt base damage (1-2-1)
- Per-attack-type (Left/Right/Up/Down attack, Riposte, combo finisher, MasterStrike) Thrust/Slash/Blunt multipliers + Priority HP Ratio (1-2-2)
- Per-enemy-type Thrust/Slash/Blunt defense — flat reduction value, not a ratio (1-2-3)

---

