# Implementation index

This directory is the **present** in the [docs lifecycle](../README.md#the-lifecycle): it tracks what is currently being worked on, and documents SW as it is actually built. Content here describes real code in `src/` and the work in flight to finish it. It should not restate the proposals in [docs/proposal](../proposal/README.md) (the plan) or the standards in [docs/foundational](../foundational/README.md) (the settled facts); it links to them and reports where the code stands against them.

When a piece of work here is finished and verified, the resulting facts move into [docs/foundational](../foundational/README.md) and the item is closed out below.

## Contents

- [STAGES.md](STAGES.md) — the staged plan for finishing the day-zero scope: ordered stages, exit criteria, and the decisions each stage needs from the maintainer.
- This README — build and development setup, and a snapshot of what is real versus outstanding.

## What is being worked on

The work in flight is the head and day-zero widget set from [proposal 0003](../proposal/0003-day-zero-widget-set.md) — `sw_application`, `sw_window`, `SwWidget`, `SwBox`, `SwLabel`, `SwButton`, `SwEntry` — implementing the lifecycle model from [proposal 0002](../proposal/0002-lifecycle-ownership-model.md). Per [SYSTEM_DESIGN.md](../foundational/SYSTEM_DESIGN.md#16-development-phases) the project is in early Phase 1 (foundation): the repository layout and Meson build exist and the source for all seven types is written. What is missing is the evidence that it works to the standard in [Architecture.md section 4](../foundational/Architecture.md#4-testing-gate-for-release).

## Build and development setup

Dependencies: a C toolchain, Meson (`>= 0.60`), GTK4 (`>= 4.10`) and GLib/GObject (`>= 2.70`) development packages.

```sh
meson setup build
meson compile -C build
meson test -C build       # runs under xvfb-run if available
meson install -C build    # installs headers under $includedir/sw-1.0 and a sw-1.0.pc
```

## Component status

| Component | Source | Tests | Day-zero criteria demonstrated |
|---|---|---|---|
| `core/` (init, version, error) | Written | Unit tests in `src/tests/unit/test-core.c` | n/a (infrastructure) |
| `style/` (metrics tokens) | Written | Covered indirectly by `test-core.c` | Contrast not yet checked |
| `SwLifecycleObserver`, `SwWindow`, `SwApplication` | Written | None | No |
| `SwWidget` (base) | Written | None | No |
| `SwBox` | Written | None | No |
| `SwLabel` | Written | None | No |
| `SwButton` | Written | None | No |
| `SwEntry` | Written | None | No |
| Showcase application | Not started | — | — |

## What's real versus what's outstanding

**Real:**

- Source for the head and all five day-zero widget types, organised by module under `src/framework/` (see [src/framework/README.md](../../src/framework/README.md)).
- A Meson build that compiles `libsw-1.0` with `-Wall -Wextra` and installs a `sw-1.0.pc`, so consumers can build against it with `pkg-config --cflags --libs sw-1.0`.
- Public headers documented inline with ownership and thread-affinity, per [Architecture.md section 5](../foundational/Architecture.md#5-documentation-standard).
- A unit test for `core/` only (`src/tests/unit/test-core.c`).

**Outstanding** (each is a stage in [STAGES.md](STAGES.md)):

- Widget, accessibility, and visual-regression tests for `SwWidget`, `SwBox`, `SwLabel`, `SwButton`, and `SwEntry`.
- The [proposal 0003 section 7](../proposal/0003-day-zero-widget-set.md#7-success-criteria-for-day-zero) success criteria: the full lifecycle firing in order, a real `SwEntry` commit-on-`UNFOCUSED` observer integration, keyboard-only operation across `SwButton`/`SwEntry`, a real screen-reader review, and checked (not assumed) contrast.
- A showcase application (SYSTEM_DESIGN.md Phase 1) to host the demonstration.
- A generated public API reference. Public headers are documented inline today; no generated reference is built from them.

None of the day-zero widgets is release-gated-complete per [Architecture.md section 4](../foundational/Architecture.md#4-testing-gate-for-release).

## Divergence from the proposals

Record here any place the implementation has deliberately departed from a proposal or the foundational documents, and why. Nothing is recorded yet. An accidental mismatch is a bug and belongs in [docs/bugs](../bugs/README.md).

See [src/framework/README.md](../../src/framework/README.md) and [src/tests/README.md](../../src/tests/README.md) for the current state of the source tree itself.
