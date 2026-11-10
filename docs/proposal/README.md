# Proposal index

This directory is the **future** in the [docs lifecycle](../README.md#the-lifecycle): every planned change is logged here first, as a formal, reviewed proposal, and will be done at some point. Nothing is built without a proposal. When work on a proposal starts, its live status is tracked in [docs/Implementation](../Implementation/README.md); the proposal stays here as the record of *why*. When the work is done and verified, the resulting facts are written into [docs/foundational](../foundational/README.md). Ideas that are not yet committed work belong in [docs/Farfuture](../Farfuture/README.md) instead.

A proposal records a concrete decision about SW's API, architecture, or scope that goes beyond the general direction set out in [docs/foundational](../foundational/README.md).

## Purpose

The foundational documents describe SW's overall intent and a proposed architecture. They are deliberately high-level and are not meant to capture every detail or to be revisited for each individual design decision. This directory is where those individual decisions should live once they need to be made explicitly — for example:

- A specific component's public API before it is implemented.
- A change to an architectural decision recorded in [SYSTEM_DESIGN.md](../foundational/SYSTEM_DESIGN.md).
- A resolution to one of the open decisions listed in SYSTEM_DESIGN.md section 18 (license, minimum GTK4 version, ABI policy, theme customization surface, and similar).
- A new subsystem or dependency that was not part of the original proposal.

## What a proposal should contain

A proposal does not need a fixed template yet, but it should generally cover:

- The problem being solved and why it matters.
- The proposed approach, and any alternatives considered.
- Which ownership boundary it falls under (see SYSTEM_DESIGN.md section 3).
- Compatibility or migration impact, if any.
- Open questions that still need resolution.

## Proposals

"Accepted" means the decision is made; it does not mean the work is finished. The Implementation column says where the work stands, and links to the tracking in [docs/Implementation](../Implementation/README.md).

| # | Title | Decision | Implementation |
|---|---|---|---|
| [0001](0001-dependency-ownership-policy.md) | Dependency ownership policy — the standing "own vs. outsource" criteria, applied to GTK4 and every future dependency | Accepted | Standing policy; applied in the build and source layout. No separate work item. |
| [0002](0002-lifecycle-ownership-model.md) | Lifecycle ownership model — Android-shaped state machine and observer interface for `sw_application`/`sw_window` | Accepted | In progress: source exists (`sw-lifecycle`, `sw-window`, `sw-application`); verification outstanding — see [STAGES.md](../Implementation/STAGES.md). |
| [0003](0003-day-zero-widget-set.md) | Day-zero widget set — the head plus the absolute minimum widgets (`SwWidget`, `SwBox`, `SwLabel`, `SwButton`, `SwEntry`) needed to prove the architecture works end to end | Accepted | In progress: source exists; widget/accessibility/visual tests, the section 7 success criteria, and a showcase app are outstanding — see [STAGES.md](../Implementation/STAGES.md). |

## Adding a proposal

Further proposals follow the same shape: problem, proposed approach, ownership-boundary call per proposal 0001, compatibility impact, and open questions. Number them in sequence, add a row to the table above, and keep this README in sync. A proposal that is plausible but not yet something we intend to do goes in [Farfuture](../Farfuture/README.md) and is promoted here when it becomes committed work.
