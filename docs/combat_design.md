# Directional Parry Combat System

## Overview

A 1v1 melee combat system where the player must read enemy attack animations and respond with a directional parry. The skill expression comes from correctly identifying the attack direction *and* timing the parry precisely — rewarding attentive players with an immediate counter.

The system is inspired by KCD2's directional combat but built around a faster, more reactive feel.

---

## Three Parry States

| State | Condition | Outcome |
|---|---|---|
| **Perfect Parry** | Correct direction + correct timing | Auto-triggered riposte animation; damage applied to enemy |
| **Partial Parry** | Wrong direction + correct timing | Slight advantage over Block (TBD); enemy continues attacking |
| **Block** | Wrong timing (any direction) | No advantage; enemy continues attacking |

### Notes
- On **Perfect Parry**, the player character immediately enters a dedicated riposte animation — this is automatic, not player-initiated.
- The **Partial Parry advantage** is not yet determined. Candidates: shorter recovery time, small stamina gain, brief stagger on enemy
- **Block** and **Partial Parry** both allow the enemy to continue their attack pattern (including combos).

---

## Directional System

### 4 Directions
`Up`, `Down`, `Left`, `Right`

### How Direction Is Determined
Each enemy attack has a directional enum assigned. The attack animation mirrors this enum visually so the player can read it without a UI indicator.

| Attack Enum | Animation Example | Correct Parry |
|---|---|---|
| `Up` | Overhead downward swing | Player inputs `Up` |
| `Down` | Low sweep or rising strike | Player inputs `Down` |
| `Left` | Swing coming from enemy's left (player's right) | Player inputs `Left` |
| `Right` | Swing coming from enemy's right (player's left) | Player inputs `Right` |

There is no UI hint. The player must read the enemy animation. This creates a high skill ceiling — mastery means knowing each enemy's move set.

---

## Input System (PC, Keyboard & Mouse)

### Parry Input
- Hold a **WASD direction** → indicates intended parry direction
- Press **Parry button** → triggers parry attempt
- The direction registered is the WASD input at the exact frame the parry button is pressed.

| WASD Input | Parry Direction |
|---|---|
| W (forward) | Up |
| S (backward) | Down |
| A (left) | Left |
| D (right) | Right |
| No direction held | Block (no directional intent) |

---

## Enemy Attack System

Each enemy attack is defined by:
- **Direction enum**: `Up`, `Down`, `Left`, `Right`
- **Parry window**: The timing window during which a parry attempt is evaluated
- **Animation**: Must visually telegraph the direction clearly before the window opens

### Combo Sequences
Enemies can chain attacks with different directions (e.g., `Left` → `Right` → `Up`). Each attack in a combo has its own parry window and direction. A Perfect Parry on any hit in the chain triggers the riposte and breaks the combo.

---

## Open Questions / TBD

- **Partial Parry advantage**: What specifically does the player gain vs. a plain Block?
- **Parry timing window size**: How tight? (Souls-like precision vs. more forgiving?)
- **Riposte animation variety**: Does the riposte animation vary by parry direction, or is it always the same regardless?
- **No-direction Block behavior**: Should holding no direction make parry impossible, or default to a direction (e.g., always `Up`)?
- **Stamina/resource system**: Does parrying consume a resource? Does a Block drain more than a Perfect Parry?
- **Failed parry (too early/late)**: Does a mistimed parry leave the player vulnerable, or just fail silently?
