# Directional Parry — 2-Week Implementation Plan

Full design reference: `docs/combat_design.md`

The existing codebase (`Variant_Combat`) already has a working combat loop:
combo attacks, charged attacks, damage interfaces, and a StateTree-driven AI.
This plan builds the directional parry system **on top of** that foundation.

---

## Week 1 — Foundation (Data, Input, Parry Window)

### Task 1 — Direction Enum
**Goal**: Define the shared language that everything else uses.

Create a new header file for a `EParryDirection` enum with five values:
`Up`, `Down`, `Left`, `Right`, `None`.

Both the player's parry input and the enemy's attack data will use this same enum.
Every other task in this plan depends on this existing first.

---

### Task 2 — Parry Input Action
**Goal**: The player can press a parry button and the game reads their WASD direction at that moment.

Modify `CombatCharacter` to:
- Add a new Input Action for parry (alongside the existing `ComboAttackAction`, `ChargedAttackAction`).
- Add an input handler that, when the parry button is pressed, reads the current movement input vector and converts it into a `EParryDirection` value.
- Store the result as a `CurrentParryDirection` property on the character.

No parry logic yet — just capturing the input.

---

### Task 3 — Enemy Attack Direction Property
**Goal**: Each enemy attack knows what direction it is.

Modify `CombatEnemy` to:
- Add an `AttackDirection` property of type `EParryDirection`.
- This property will be set before each attack animation plays (for now, hardcode a value — e.g., always `Up`).

This lets the parry evaluation later compare player direction against enemy direction.

---

### Task 4 — Parry Window AnimNotify (Open)
**Goal**: The enemy attack animation signals when the parry window begins.

Create a new AnimNotify class: `AnimNotify_OpenParryWindow`.

When fired, it finds the enemy character and sets a flag (`bParryWindowOpen = true`) on it.

This is the same pattern as the existing `AnimNotify_DoAttackTrace` — it fires at a specific frame in the animation.

---

### Task 5 — Parry Window AnimNotify (Close)
**Goal**: The parry window closes after the attack impact frame.

Create a new AnimNotify class: `AnimNotify_CloseParryWindow`.

When fired, it sets `bParryWindowOpen = false` on the enemy.

Together, Tasks 4 and 5 define the timing window between which the player must press parry.
Place `OpenParryWindow` just before the impact frame, and `CloseParryWindow` just after.

---

### Task 6 — Parry Window State on Enemy
**Goal**: The enemy tracks whether its window is currently open.

Modify `CombatEnemy` to:
- Add `bParryWindowOpen` boolean property (default: `false`).
- Add `AttackDirection` (from Task 3, confirm it belongs here).
- Add a method `IsParryWindowOpen()` that returns the flag — the player's parry logic will call this.

---

## Week 2 — Parry Evaluation, Riposte, AI Direction

### Task 7 — Parry State Evaluation
**Goal**: When the player presses parry, determine which of the three states occurs.

Modify `CombatCharacter` to add a `TryParry()` function that:
1. Finds the nearby enemy (within a range).
2. Asks the enemy if its parry window is open (`IsParryWindowOpen()`).
3. If **window is open**:
   - Compares `CurrentParryDirection` against the enemy's `AttackDirection`.
   - Match → **Perfect Parry**.
   - No match → **Partial Parry**.
4. If **window is closed** → **Block**.
5. Stores the result in a `LastParryResult` enum on the character.

No animations yet — just the logic that classifies the result.

---

### Task 8 — Block and Partial Parry Response
**Goal**: Block and Partial Parry play their respective animations.

Modify `CombatCharacter` to:
- Add a `BlockMontage` and a `PartialParryMontage` property.
- After `TryParry()` resolves, play the correct montage based on `LastParryResult`.
- Partial Parry advantage is **TBD** — leave a clearly marked placeholder comment for it.

---

### Task 9 — Riposte Animation (Perfect Parry Response)
**Goal**: A Perfect Parry immediately triggers the riposte animation and deals damage.

Modify `CombatCharacter` to:
- Add a `RiposteMontage` property.
- On Perfect Parry: play the riposte montage automatically.
- Reuse the existing `AnimNotify_DoAttackTrace` in the riposte montage to trigger the damage hit.
  The existing damage pipeline (`ICombatDamageable::ApplyDamage`) handles the rest.

---

### Task 10 — Enemy Attack Direction Assignment (AI)
**Goal**: Enemy attacks are assigned real directions instead of the hardcoded placeholder from Task 3.

Modify `CombatStateTreeUtility`:
- In `FStateTreeComboAttackTask`, set the enemy's `AttackDirection` before each attack plays.
- Each combo hit can have a different direction — for example, a 3-hit combo could be `Left → Right → Up`.
- Add the direction assignment alongside the existing `DoAIComboAttack()` call.

---

### Task 11 — Place AnimNotifies in Enemy Montages
**Goal**: Enemy attack montages have the parry window properly timed.

In the Unreal Editor (Animation Editor, not code):
- Open each enemy attack montage.
- Add `AnimNotify_OpenParryWindow` at the frame where the swing starts committing.
- Add `AnimNotify_CloseParryWindow` just after the impact frame.

This is a content task, not a code task — done in the editor's timeline.

---

### Task 12 — Combo Parry Testing
**Goal**: Ensure Perfect Parry mid-combo breaks the enemy combo correctly.

Verify in-game:
- Enemy performs a multi-hit combo (e.g., `Left → Right → Up`).
- A Perfect Parry on any hit triggers the riposte and stops the enemy's remaining combo.
- A Partial Parry on any hit allows the enemy to continue to the next hit.
- A Block on any hit behaves the same as Partial Parry for now (no advantage difference yet).

No new code needed here — this is integration testing of everything built so far.

---

## What Is Left for After These 2 Weeks

These are intentionally out of scope — resolve the design questions first:

- **Partial Parry exact advantage** (shorter recovery? stamina gain? enemy stagger?)
- **Failed/mistimed parry consequence** (vulnerable window, or silent failure?)
- **Riposte animation variance** (same riposte always, or direction-dependent?)
- **Stamina/resource system** (does parrying cost something?)
- **Lock-on system** (planned — will affect how the enemy reference in `TryParry()` is found)

---

## Files You Will Create or Modify

| File | Action | Task |
|---|---|---|
| `EParryDirection.h` (new) | Create enum | Task 1 |
| `CombatCharacter.h/cpp` | Add parry input, `TryParry()`, montages | Tasks 2, 7, 8, 9 |
| `CombatEnemy.h/cpp` | Add `AttackDirection`, `bParryWindowOpen` | Tasks 3, 6 |
| `AnimNotify_OpenParryWindow.h/cpp` (new) | Create AnimNotify | Task 4 |
| `AnimNotify_CloseParryWindow.h/cpp` (new) | Create AnimNotify | Task 5 |
| `CombatStateTreeUtility.h/cpp` | Assign direction in attack tasks | Task 10 |
| Enemy attack montages (Editor) | Place AnimNotify keyframes | Task 11 |
