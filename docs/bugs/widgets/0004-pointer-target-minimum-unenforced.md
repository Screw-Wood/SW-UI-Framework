# 0004: nothing enforces the 24dp pointer target minimum; SW_METRICS_* tokens are unused

**Status:** Open — blocks criterion 5's prerequisites and the Stage 4 contrast/target work
**Found:** 2026-11-10, HEAD at the time of the "Docs update" commit (built locally with GTK 4.14.5 / Meson, Ubuntu packages; measured with throwaway probe programs)
**Affects:** `src/framework/widgets/sw-button.c`, `src/framework/widgets/sw-entry.c`, `src/framework/style/sw-metrics.h`

## What happens

A bare `SwButton` measures 8x17 (device-independent pixels) and a bare `SwEntry` measures 17 tall. Neither widget's minimum size is clamped to `SW_METRICS_TARGET_POINTER_MIN_DP` (24). Grepping the widget sources, none of the `SW_METRICS_*` tokens declared in `sw-metrics.h` (grid baseline, fine grid, pointer/touch target minimums, screen-edge margin) is referenced anywhere outside that header itself.

## What you expected

[design-doc.md section 2](../../foundational/design-doc.md#2-spacing-and-layout-grid), via [Architecture.md section 6](../../foundational/Architecture.md#6-accessibility-compliance-baseline), sets a 24x24dp minimum pointer target. [STAGES.md Stage 3](../../Implementation/STAGES.md#stage-3--accessibility-tests) lists checking interactive target sizes against that threshold as part of the accessibility tier; it cannot pass while no widget ever requests that minimum size in the first place.

## How to reproduce

1. Build and run a probe program constructing a bare `SwButton` (no label) and a bare `SwEntry`.
2. Measure their natural/minimum allocated size.
3. Observed: 8x17 for the button, 17 tall for the entry, both well under 24dp on at least one axis. Expected: both widgets request at least 24x24dp.

## Notes

This is as much a gap as a bug: the tokens exist in `sw-metrics.h` but nothing in `widgets/` consumes them, so there is currently no path by which a widget's minimum size could reflect the design system's target-size requirement even accidentally. Filed here (rather than only noted as a gap) because it concretely blocks part of [STAGES.md Stage 3](../../Implementation/STAGES.md#stage-3--accessibility-tests) and the target-size half of [STAGES.md Stage 4](../../Implementation/STAGES.md#stage-4--contrast-checking)'s scope. No fix has been made to `src/`.
