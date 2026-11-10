# Docs index

This directory holds SW's project documentation. It is organised by **where a piece of knowledge sits in time**: what is already settled, what is being built now, what is planned, and what is only a distant possibility. A document's directory tells you how much to trust it and how soon it may change.

## The lifecycle

```text
Farfuture  →  proposal  →  Implementation  →  foundational
 (maybe)      (will do)      (doing now)        (done, fact)
```

| Directory | Time | What lives here | Who reads it |
|---|---|---|---|
| [foundational/](foundational/README.md) | **Past** — settled facts | The idea, research, architecture, standards, and design-system values that are decided and in force. | Everyone. **This is the directory users and newcomers should read.** |
| [Implementation/](Implementation/README.md) | **Present** — being worked on | What is actually in `src/` today, the staged plan for the work currently in flight, and its status. | Contributors working on the code. |
| [proposal/](proposal/README.md) | **Future** — will be done at some point | Every planned change is logged here first: new features, API additions, architecture changes. | Contributors and reviewers. |
| [Farfuture/](Farfuture/README.md) | **Far future** — might be done | Ideas that are not yet committed work and have no schedule. | Anyone curious about direction. |

Alongside the lifecycle, one directory tracks what goes wrong, whatever stage the affected code is in:

| Directory | What lives here |
|---|---|
| [bugs/](bugs/README.md) | Bug reports, filed by **where the bug happens** (one subdirectory per area of the codebase, plus `tests`, `unexpected`, and `other`). |

### How a document moves

1. **Everything starts as a proposal** (or in `Farfuture/` if it is not yet something we intend to do). Nothing is built without being logged in `proposal/` first.
2. **When work starts**, the proposal is tracked in `Implementation/`: what exists, what is outstanding, and the stages remaining. The proposal itself stays in `proposal/` as the record of *why*.
3. **When the work is done and verified**, the resulting facts are written into `foundational/`. That is the only point at which something becomes a fact users can rely on.
4. **Bugs** are logged in `bugs/` under the area where they occur, whatever stage the affected code is in.

Content moves forward, never silently backward: if something in `foundational/` turns out to be wrong, record the correction in `bugs/` or a new proposal rather than quietly rewriting history.

## Foundational

- [Foundation index](foundational/README.md)
  - [The idea](foundational/IDEA.md) — what SW is, why it exists, who it may help, and what success looks like.
  - [Research and references](foundational/RESEARCH.md) — existing projects, technical foundations, and primary references.
  - [System design](foundational/SYSTEM_DESIGN.md) — architecture, ownership boundaries, platform integration, and development phases.
  - [Architecture](foundational/Architecture.md) — the v1 standard: versioning/ABI policy, testing and accessibility release gates, and what "corporate MVP" means for v1.
  - [design-doc](foundational/design-doc.md) — concrete numeric design-system values (spacing, elevation, contrast, typography) for v1's default theme.

## Implementation

- [Implementation index](Implementation/README.md) — the state of the codebase: build instructions, what is real versus outstanding.
  - [Staged implementation plan](Implementation/STAGES.md) — the ordered stages to finish the day-zero scope, with exit criteria and the decisions each stage needs from the maintainer.

## Proposal

- [Proposal index](proposal/README.md) — every planned change, logged before it is built.
  - [0001 — Dependency ownership policy](proposal/0001-dependency-ownership-policy.md) — the standing "own vs. outsource" criteria applied to GTK4 and every future dependency.
  - [0002 — Lifecycle ownership model](proposal/0002-lifecycle-ownership-model.md) — the Android-shaped state machine and observer interface for `sw_application`/`sw_window`.
  - [0003 — Day-zero widget set](proposal/0003-day-zero-widget-set.md) — the head plus the minimum widget set (`SwWidget`, `SwBox`, `SwLabel`, `SwButton`, `SwEntry`) that proves the architecture end to end.

## Farfuture

- [Farfuture index](Farfuture/README.md) — long-range ideas with no commitment or schedule.

## Bugs

- [Bugs index](bugs/README.md) — how to file a bug, and the area subdirectories: [core](bugs/core/README.md), [application](bugs/application/README.md), [widgets](bugs/widgets/README.md), [layout](bugs/layout/README.md), [style](bugs/style/README.md), [accessibility](bugs/accessibility/README.md), [platform](bugs/platform/README.md), [tests](bugs/tests/README.md), [unexpected](bugs/unexpected/README.md), [other](bugs/other/README.md).

## Where the existing documents sit

The three accepted proposals ([0001](proposal/0001-dependency-ownership-policy.md)–[0003](proposal/0003-day-zero-widget-set.md)) were written before this lifecycle existed and remain in `proposal/` as the record of why each decision was made. Their work is now **in progress**, so their live status is tracked in [Implementation](Implementation/README.md), not in the proposals themselves. The foundational documents were likewise written as a proposal-stage description; they are the authoritative reference for the project's intent and standards, and Implementation is authoritative for what the code currently does.
