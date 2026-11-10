# Docs index

This directory holds SW's project documentation. It is organised by the kind of material each document contains, not by implementation detail.

## Foundational

- [Foundation index](foundational/README.md)
  - [The idea](foundational/IDEA.md) — what SW is, why it exists, who it may help, and what success looks like.
  - [Research and references](foundational/RESEARCH.md) — existing projects, technical foundations, and primary references.
  - [System design](foundational/SYSTEM_DESIGN.md) — proposed architecture, ownership boundaries, platform integration, and development phases.
  - [Architecture](foundational/Architecture.md) — the v1 standard: versioning/ABI policy, testing and accessibility release gates, and what "corporate MVP" means for v1.
  - [design-doc](foundational/design-doc.md) — concrete numeric design-system values (spacing, elevation, contrast, typography) for v1's default theme.

## Proposal

- [Proposal index](proposal/README.md) — formal, accepted proposals for features, architecture changes, or API additions.
  - [0001 — Dependency ownership policy](proposal/0001-dependency-ownership-policy.md) — the standing "own vs. outsource" criteria applied to GTK4 and every future dependency.
  - [0002 — Lifecycle ownership model](proposal/0002-lifecycle-ownership-model.md) — the Android-shaped state machine and observer interface for `sw_application`/`sw_window`.
  - [0003 — Day-zero widget set](proposal/0003-day-zero-widget-set.md) — the head plus the minimum widget set (`SwWidget`, `SwBox`, `SwLabel`, `SwButton`, `SwEntry`) that proves the architecture end to end.

## Implementation

- [Implementation index](Implementation/README.md) — documentation that tracks the actual state of the codebase (module layout, build instructions, API reference) as it is written.

## How these relate

The foundational documents describe the project as proposed: why it exists and how it is intended to be built. Proposal documents record specific, reviewed decisions as they are made — two are accepted so far, covering dependency ownership and the application/window lifecycle. Implementation documents will describe what has actually been built, and should stay in sync with the code rather than the original proposal.

Until there is a working prototype, foundational documents are the most current description of the project. Implementation documentation should only describe what exists in `src/`; it should not restate the proposal.
