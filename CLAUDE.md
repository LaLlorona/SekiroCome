# SekiroCome — Claude Context

## IMPORTANT: Unreal Engine Instruction Guidelines

The user is a beginner in Unreal Engine. Follow these principles at all times:

- **Read files immediately** without asking for permission when needed.
- **Do not provide too much code at once.** Use the step-by-step process below.

### Step 1 — Task Presentation (Default)
Explain what needs to be created. During this stage:
- Do not write any code.
- Do not explain "how to write the code" yet.
- Only specify which files, classes, or assets need to be created.
- Briefly explain why this task is necessary.

### Step 2 — Hints (If the user is stuck)
If the user says things like "I'm not sure", "How do I do this?", "I'm stuck", or "I don't know":
- Provide a step-by-step explanation of where and how to create it in Unreal Engine.
- Show only examples of core parts — not the full code.

### Step 3 — Full Code (Only on explicit request)
Provide complete, finished code only when the user explicitly asks with phrases like:
- "Show me the code."
- "Give me the full code."

---

## Project Goal
A 3D melee action game (Unreal Engine) focused on a skill-expressive 1v1 combat system built around directional parrying. The name references Sekiro's parry-centric feel.

## Combat System: Directional Parry

Full design doc: `docs/combat_design.md`

### Three Parry States
- **Perfect Parry**: correct direction + correct timing → auto-triggered riposte animation + damage
- **Partial Parry**: wrong direction + correct timing → slight advantage (TBD), enemy continues
- **Block**: wrong timing → no advantage, enemy continues

### Directions
4-directional: `Up`, `Down`, `Left`, `Right` — defined as an enum on enemy attacks. No UI indicators; player reads enemy animations.

### Input (PC only)
- WASD direction held at the moment of pressing the parry button = parry direction
- No direction held = Block (no directional intent)

### Enemy Attacks
Each attack has a direction enum + animation that telegraphs the direction. Enemies support combo chains where each hit has its own direction and parry window.

## Key Terminology
| Term | Meaning |
|---|---|
| Perfect Parry | Correct direction + correct timing |
| Partial Parry | Wrong direction + correct timing |
| Block | Wrong timing |
| Riposte | Auto-triggered counter animation after Perfect Parry |

## TBD / Open Design Questions
- Partial Parry exact advantage over Block
- Parry timing window size
- Whether riposte animation varies by parry direction
- Stamina/resource system
- Consequence of a mistimed parry attempt (vulnerable window?)
