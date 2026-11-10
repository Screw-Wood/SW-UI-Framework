# 0003: each SwEntry is two Tab stops, and the outer one accepts no typing

**Status:** Open — blocks criterion 3
**Found:** 2026-11-10, HEAD at the time of the "Docs update" commit (built locally with GTK 4.14.5 / Meson, Ubuntu packages; driven with real key events via `xdotool` on Xvfb)
**Affects:** `src/framework/widgets/sw-entry.c`, `src/framework/widgets/sw-entry.h`

## What happens

Tabbing through a layout that contains one `SwEntry` stops on it twice in a row: an outer focus stop (on `SwEntry` itself, or a wrapping container it composes) and an inner one (the actual text-editing widget it wraps). The outer stop accepts keyboard focus but typing while on it does nothing; only the inner stop actually edits text.

## What you expected

[Proposal 0003 section 7](../../proposal/0003-day-zero-widget-set.md#7-success-criteria-for-day-zero) criterion 3 requires keyboard-only operation "across `SwButton`/`SwEntry`" with a sane focus order; a single logical widget occupying two Tab stops, one of them non-functional, is not a sane focus order and will also read oddly to assistive technology (see [STAGES.md Stage 3](../../Implementation/STAGES.md#stage-3--accessibility-tests)).

## How to reproduce

1. Build and run any program composing an `SwEntry` into a focusable layout alongside at least one other focusable widget.
2. Tab through the layout.
3. Observed: two consecutive Tab stops land on/in the entry, the first of which does not accept typed input. Expected: one Tab stop, which accepts typed input immediately.

## Notes

`SwEntry` is built by wrapping an internal text-editing widget; this looks like the wrapper itself is left focusable (e.g. `can-focus` not cleared, or no `gtk_widget_set_focus_child`/delegate-focus wiring to the inner widget) instead of delegating focus straight through to the thing that actually edits text. No fix has been made to `src/`.
