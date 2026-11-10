# Docs index

This directory holds SW's project documentation. It is organised by the kind of material each document contains, not by implementation detail.

## Foundational

- [Foundation index](foundational/README.md)
  - [The idea](foundational/IDEA.md) — what SW is, why it exists, who it may help, and what success looks like.
  - [Research and references](foundational/RESEARCH.md) — existing projects, technical foundations, and primary references.
  - [System design](foundational/SYSTEM_DESIGN.md) — proposed architecture, ownership boundaries, platform integration, and development phases.

## Proposal

- [Proposal index](proposal/README.md) — formal proposals for features, architecture changes, or API additions, once the project has concrete decisions to record.

## Implementation

- [Implementation index](Implementation/README.md) — documentation that tracks the actual state of the codebase (module layout, build instructions, API reference) as it is written.

## How these relate

The foundational documents describe the project as proposed: why it exists and how it is intended to be built. Proposal documents will record specific, reviewed decisions as they are made. Implementation documents will describe what has actually been built, and should stay in sync with the code rather than the original proposal.

Until there is a working prototype, foundational documents are the most current description of the project. Implementation documentation should only describe what exists in `src/`; it should not restate the proposal.
