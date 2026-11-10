# Implementation index

This directory is the **present** in the [docs lifecycle](../README.md#the-lifecycle): it tracks what is currently being worked on, and documents SW as it is actually built. Content here describes real code in `src/` and the work in flight to finish it. It should not restate the proposals in [docs/proposal](../proposal/README.md) (the plan) or the standards in [docs/foundational](../foundational/README.md) (the settled facts); it links to them and reports where the code stands against them.

When a piece of work here is finished and verified, the resulting facts move into [docs/foundational](../foundational/README.md) and the item is closed out below.

## Contents

- [STAGES.md](STAGES.md) — the staged plan for finishing the day-zero scope: ordered stages, exit criteria, and the decisions each stage needs from the maintainer.
- This README — build and development setup, and a snapshot of what is real versus outstanding.

## What is being worked on

The work in flight is the head and day-zero widget set from [proposal 0003](../proposal/0003-day-zero-widget-set.md) — `sw_application`, `sw_window`, `SwWidget`, `SwBox`, `SwLabel`, `SwButton`, `SwEntry` — implementing the lifecycle model from [proposal 0002](../proposal/0002-lifecycle-ownership-model.md). Per [SYSTEM_DESIGN.md](../foundational/SYSTEM_DESIGN.md#16-development-phases) the project is in early Phase 1 (foundation): the repository layout and Meson build exist and the source for all seven types is written. What is missing is the evidence that it works to the standard in [Architecture.md section 4](../foundational/Architecture.md#4-testing-gate-for-release); [STAGES.md](STAGES.md) is the plan for producing that evidence as automated, repeatable tests.

A manual verification pass (building the tree directly and probing it with throwaway programs, plus real key/pointer events via `xdotool` on Xvfb) ran ahead of any of those stages having its own test tier. It is not a substitute for Stages 1-3 and is not checked into the repository; it found the component-status snapshot below, and the eight defects filed in [docs/bugs](../bugs/README.md).

## Build and development setup

Dependencies: a C toolchain, Meson (`>= 0.60`), GTK4 (`>= 4.10`) and GLib/GObject (`>= 2.70`) development packages.

```sh
meson setup build
meson compile -C build
meson test -C build       # runs under xvfb-run if available
meson install -C build    # installs headers under $includedir/sw-1.0 and a sw-1.0.pc
```

## Component status

| Component | Source | Tests | Day-zero criteria demonstrated | Manually observed (not yet an automated test) |
|---|---|---|---|---|
| `core/` (init, version, error) | Written | Unit tests in `src/tests/unit/test-core.c`, passing; clean under `-Wall -Wextra` (GCC 13.3.0) and ASan/UBSan | n/a (infrastructure) | — |
| `style/` (metrics tokens) | Written | Covered indirectly by `test-core.c` | Contrast not yet checked | [widgets/0004](../bugs/widgets/0004-pointer-target-minimum-unenforced.md): tokens exist but no widget consumes them |
| `SwLifecycleObserver`, `SwWindow`, `SwApplication` | Written | None | Partial | Forward/backward lifecycle order and `FOCUSED ⇄ UNFOCUSED` cycling confirmed correct under ASan/UBSan; [application/0001](../bugs/application/0001-observer-self-removal-skips-next.md) and [application/0002](../bugs/application/0002-state-stale-after-hide-reshow.md) found open |
| `SwWidget` (base) | Written | None | No | Accessible role present on every concrete type |
| `SwBox` | Written | None | No | — |
| `SwLabel` | Written | None | No | — |
| `SwButton` | Written | None | Partial | Pointer click, hover, and pressed states work; [widgets/0001](../bugs/widgets/0001-button-keyboard-activation-noop.md) (no keyboard activation) and [widgets/0002](../bugs/widgets/0002-button-clicked-outside-release.md) (clicks on release outside) found open |
| `SwEntry` | Written | None | Partial | Commits on window `UNFOCUSED`, not on programmatic `sw_entry_set_text`, as designed; [widgets/0003](../bugs/widgets/0003-entry-double-tab-stop.md) and [widgets/0005](../bugs/widgets/0005-entry-committed-fires-on-own-focus-out.md) found open |
| Showcase application | Not started | — | — | — |

The "Manually observed" column records a one-off verification pass (built and run locally with GTK 4.14.5/Meson on Ubuntu, throwaway probe programs, and `xdotool`-driven key/pointer events on Xvfb) that is not checked into this repository and is not a substitute for the automated tiers in [STAGES.md](STAGES.md) — it is why each row above now has more detail than "Written/None/No" without any stage being complete.

## What's real versus what's outstanding

**Real:**

- Source for the head and all five day-zero widget types, organised by module under `src/framework/` (see [src/framework/README.md](../../src/framework/README.md)).
- A Meson build that compiles `libsw-1.0` with `-Wall -Wextra`, installs a `sw-1.0.pc`, and is clean under ASan/UBSan on the `core` test and the manual lifecycle probes above.
- Public headers documented inline with ownership and thread-affinity, per [Architecture.md section 5](../foundational/Architecture.md#5-documentation-standard).
- A unit test for `core/` only (`src/tests/unit/test-core.c`).
- The lifecycle's forward and backward ordering, `FOCUSED ⇄ UNFOCUSED` cycling, `SwEntry`'s commit-on-`UNFOCUSED` behaviour, `SwButton`'s pointer click/hover/pressed states, and a real accessible role on every type — all confirmed by manual verification, none yet by an automated test.

**Not real, corrected from an earlier claim:** installing and building against `libsw-1.0` with plain `pkg-config --cflags --libs sw-1.0` does **not** work for a consumer that also calls GTK/GLib/GObject directly — see [other/0001](../bugs/other/0001-pc-file-requires-private-gtk.md). The installed `sw-1.0.pc` lists those as `Requires.private`.

**Outstanding** (each is a stage in [STAGES.md](STAGES.md)):

- Widget, accessibility, and visual-regression tests for `SwWidget`, `SwBox`, `SwLabel`, `SwButton`, and `SwEntry` (Stages 1, 3, 7).
- A default theme (Stage 4): there is no stylesheet and no colour values pinned anywhere, so none of the day-zero types can be contrast-checked yet.
- The [proposal 0003 section 7](../proposal/0003-day-zero-widget-set.md#7-success-criteria-for-day-zero) success criteria as *automated, repeatable* evidence rather than one-off manual observation: the full lifecycle firing in order, a real `SwEntry` commit-on-`UNFOCUSED` observer integration, keyboard-only operation across `SwButton`/`SwEntry`, a real screen-reader review, and checked (not assumed) contrast. Two of these (keyboard-only operation, checked contrast) are additionally blocked outright by open bugs and the missing theme, not just by missing tests — see [docs/bugs](../bugs/README.md).
- A showcase application (SYSTEM_DESIGN.md Phase 1) to host the demonstration (Stage 6).
- A generated public API reference. Public headers are documented inline today; no generated reference is built from them.
- Continuous integration running the sanitizer builds that [Architecture.md section 3](../foundational/Architecture.md#3-versioning-and-abi-policy) requires; today that only happened as part of the one-off manual pass above.

None of the day-zero widgets is release-gated-complete per [Architecture.md section 4](../foundational/Architecture.md#4-testing-gate-for-release). Eight defects found during manual verification are open in [docs/bugs](../bugs/README.md) and are not fixed.

## Divergence from the proposals

Record here any place the implementation has deliberately departed from a proposal or the foundational documents, and why. Nothing is recorded yet. An accidental mismatch is a bug and belongs in [docs/bugs](../bugs/README.md).

See [src/framework/README.md](../../src/framework/README.md) and [src/tests/README.md](../../src/tests/README.md) for the current state of the source tree itself.
