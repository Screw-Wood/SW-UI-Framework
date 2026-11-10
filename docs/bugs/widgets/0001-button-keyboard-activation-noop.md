# 0001: SwButton keyboard activation does nothing, and warns on every key press

**Status:** Open — blocks criterion 3
**Found:** 2026-11-10, HEAD at the time of the "Docs update" commit (built locally with GTK 4.14.5 / Meson, Ubuntu packages; driven with real key events via `xdotool` on Xvfb)
**Affects:** `src/framework/widgets/sw-button.c`, `src/framework/widgets/sw-button.h`

## What happens

Pressing Space or Enter while an `SwButton` has keyboard focus does nothing: no `clicked` signal, no visual pressed state. GTK logs a warning on every such key press.

## What you expected

[Proposal 0003 section 7](../../proposal/0003-day-zero-widget-set.md#7-success-criteria-for-day-zero) criterion 3 is keyboard-only operation across `SwButton`/`SwEntry`. A button that cannot be activated from the keyboard fails that criterion outright, independent of anything in [STAGES.md Stage 3](../../Implementation/STAGES.md#stage-3--accessibility-tests).

## How to reproduce

1. Build and run any program constructing an `SwButton` inside a focusable window.
2. Tab to give the button keyboard focus.
3. Press Space or Enter.
4. Observed: nothing happens, and a GTK warning is logged. Expected: the button activates like a mouse click would.

## Notes

Reading `sw-button.c`, `clicked` is declared as a plain signal, not installed as a `GtkWidgetClass` action signal (the mechanism GTK's default keymap bindings for buttons rely on to invoke `clicked` from Space/Enter). Without that, GTK has no action to bind the key presses to, which is also the likely source of the warning. No fix has been made to `src/`; this needs to go through whatever review process installing a new action signal on a public type requires.
