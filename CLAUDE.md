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

## Implementation Checklist Rule

Implementation progress is tracked in `docs/implementation_checklist.md`.

**When an implementation task is marked done:**
1. Read the relevant source files to verify the feature is actually present in the code.
2. Only then change `[ ]` → `[x]` in the checklist.
3. Never mark an item complete based on conversation alone — always confirm from the code.

---

## Project Goal
A 3D melee action game (Unreal Engine) focused on a skill-expressive 1v1 combat system built around directional parrying. The name references Sekiro's parry-centric feel.

## Combat System Overview

Full design doc: `docs/combatsystem_base.md`

Key systems defined in the design doc:

- **Player Actions**: Movement, Attack, Guard, Dodge — all other systems branch from these four.
- **HP / Stamina**: Both cap at 100. Damage hits Stamina first; overflow transfers to HP in the same hit. Stamina max scales with HP (`30 + 70 * HP/100`). Regen stops after Attack/Guard/Dodge/Sprint/Jump for `SP_RegenDelay` seconds; walking does not block regen.
- **Attack Direction**: 4 directions (Left/Right/Up/Down). Auto-switches after each attack or successful block per fixed tables (Section 3-1/3-2). Asymmetry is intentional — Down/Right are "easy" MasterStrike paths; Up/Left require manual re-aim.
- **Combo System**: Command-matching against a history buffer of recent attack directions. Patterns registered in a DataTable (`FCombatComboRow`). Buffer resets on enemy PerfectBlock or timeout (`Combo_BufferTimeout`).
- **MasterStrike / PerfectBlock**: Timing window is InGameTime-based (Time Dilation corrected), defined per weapon in DataTable. Finisher and MasterStrike both use a two-part damage structure: base damage (Stamina-first) + priority HP damage.
- **All tunable values** must be DataTable constants — never hardcode. Full list in Section 7 of the design doc.


