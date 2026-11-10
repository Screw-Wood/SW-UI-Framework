# Bugs

This directory records bugs, filed by **where the bug happens**. It sits alongside the [docs lifecycle](../README.md#the-lifecycle) rather than inside it: a bug can be found in code at any stage.

## Where to file a bug

Put the report in the subdirectory for the area where the bug *occurs*, not where you think the fix belongs. The areas match the modules under [`src/framework/`](../../src/framework/README.md), plus three catch-all categories.

| Directory | Covers |
|---|---|
| [core/](core/README.md) | `src/framework/core/`: init, version, error |
| [application/](application/README.md) | `src/framework/application/`: `SwApplication`, `SwWindow`, the lifecycle observer |
| [widgets/](widgets/README.md) | `src/framework/widgets/`: `SwWidget`, `SwLabel`, `SwButton`, `SwEntry` |
| [layout/](layout/README.md) | `src/framework/layout/`: `SwBox` |
| [style/](style/README.md) | `src/framework/style/`: metrics and design tokens, including contrast |
| [accessibility/](accessibility/README.md) | `src/framework/accessibility/`: roles, names, states, keyboard-only operation |
| [platform/](platform/README.md) | `src/framework/platform/`: the GTK4 bridge and platform behaviour |
| [tests/](tests/README.md) | The test suite itself in `src/tests/`: wrong, flaky, or missing tests |
| [unexpected/](unexpected/README.md) | Behaviour that surprised you and that you cannot yet place in an area |
| [other/](other/README.md) | Everything else: build system, packaging, documentation errors |

If a bug spans areas, file it where it first becomes visible and link it from the others. If you cannot tell where it belongs, use `unexpected/`; it can be moved once the cause is known.

A documentation error, or a disagreement between the code and the [foundational](../foundational/README.md) documents, goes in `other/` (or the area it concerns).

## Open bugs

As of the day-zero verification pass recorded in [Implementation/README.md](../Implementation/README.md), none of the day-zero types is release-gated-complete; these are the specific defects found so far, by area:

| Bug | Area | Blocks |
|---|---|---|
| [application/0001](application/0001-observer-self-removal-skips-next.md) — self-removing observer skips the next observer | [application/](application/README.md) | — |
| [application/0002](application/0002-state-stale-after-hide-reshow.md) — state stays SHOWN after hide/re-show | [application/](application/README.md) | Needs re-check under a real window manager |
| [widgets/0001](widgets/0001-button-keyboard-activation-noop.md) — SwButton keyboard activation is a no-op | [widgets/](widgets/README.md) | Day-zero criterion 3 |
| [widgets/0002](widgets/0002-button-clicked-outside-release.md) — SwButton clicks on release outside the button | [widgets/](widgets/README.md) | — |
| [widgets/0003](widgets/0003-entry-double-tab-stop.md) — SwEntry is two Tab stops | [widgets/](widgets/README.md) | Day-zero criterion 3 |
| [widgets/0004](widgets/0004-pointer-target-minimum-unenforced.md) — 24dp pointer target not enforced | [widgets/](widgets/README.md) | [STAGES.md](../Implementation/STAGES.md) Stages 3-4 |
| [widgets/0005](widgets/0005-entry-committed-fires-on-own-focus-out.md) — SwEntry::committed also fires on own focus-out | [widgets/](widgets/README.md) | — |
| [other/0001](other/0001-pc-file-requires-private-gtk.md) — sw-1.0.pc puts GTK under Requires.private | [other/](other/README.md) | — |

What currently works end to end — the full lifecycle in order, `SwEntry` commit-on-`UNFOCUSED`, pointer clicks and hover/pressed states, and accessible roles on every type — is recorded in [Implementation/README.md](../Implementation/README.md), not here.

## How to file one

One file per bug, in the right subdirectory, named `NNNN-short-description.md`, where `NNNN` is the next unused number *within that subdirectory*. Use this shape:

```markdown
# NNNN: short description

**Status:** Open | Fixed | Won't fix | Duplicate of <link>
**Found:** date, and the version or commit
**Affects:** the file(s) or type(s)

## What happens
What you observed.

## What you expected
What should happen, with a link to the proposal or foundational document that says so, if there is one.

## How to reproduce
Smallest steps you can give.

## Notes
Cause, workaround, links to related bugs, or the fix once there is one.
```

A bug that blocks a stage of the [staged implementation plan](../Implementation/STAGES.md) should be linked from that stage.

## Closing a bug

Do not delete a fixed bug. Set its status to *Fixed* and note the commit, so the history stays searchable. If the fix reveals that a foundational document was wrong, correct it there and say so in the bug.
